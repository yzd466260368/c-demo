#define _POSIX_C_SOURCE 200112L

#include "config.h"
#include "http.h"
#include "session.h"
#include "ssl_wrap.h"
#include "util.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define STATE_HANDSHAKE 1
#define STATE_READING   2
#define STATE_WRITING   3

struct connection {
    int fd;
    int state;
    SSL *ssl;
    char in_buf[IN_BUF_SIZE];
    size_t in_len;
    char out_buf[OUT_BUF_SIZE];
    size_t out_len;
    size_t out_sent;
};

static int g_stop = 0;

static void on_signal(int signo) {
    (void)signo;
    g_stop = 1;
}

static int create_listen_socket(int port) {
    int fd;
    int opt = 1;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(fd);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((unsigned short)port);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }

    if (listen(fd, LISTEN_BACKLOG) < 0) {
        perror("listen");
        close(fd);
        return -1;
    }

    if (set_nonblocking(fd) < 0) {
        perror("set_nonblocking listen fd");
        close(fd);
        return -1;
    }

    return fd;
}

static int epoll_add_fd(int epfd, int fd, uint32_t events, void *ptr) {
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = events;
    ev.data.ptr = ptr;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

static int epoll_mod_conn(int epfd, struct connection *c, uint32_t events) {
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = events;
    ev.data.ptr = c;
    return epoll_ctl(epfd, EPOLL_CTL_MOD, c->fd, &ev);
}

static void close_connection(int epfd, struct connection *c) {
    if (c == NULL) {
        return;
    }

    epoll_ctl(epfd, EPOLL_CTL_DEL, c->fd, NULL);

    if (c->ssl != NULL) {
        SSL_shutdown(c->ssl);
        SSL_free(c->ssl);
    }

    if (c->fd >= 0) {
        close(c->fd);
    }

    free(c);
}

static void update_interest(int epfd, struct connection *c, int ssl_error) {
    uint32_t events = 0;

    if (ssl_error == SSL_ERROR_WANT_READ) {
        events = EPOLLIN;
    } else if (ssl_error == SSL_ERROR_WANT_WRITE) {
        events = EPOLLOUT;
    } else if (c->state == STATE_HANDSHAKE) {
        events = EPOLLIN | EPOLLOUT;
    } else if (c->state == STATE_READING) {
        events = EPOLLIN;
    } else if (c->state == STATE_WRITING) {
        events = EPOLLOUT;
    }

    epoll_mod_conn(epfd, c, events);
}

static void build_bad_request(struct connection *c, const char *message) {
    size_t body_len = strlen(message);
    int n = snprintf(c->out_buf, sizeof(c->out_buf),
                     "HTTP/1.1 400 Bad Request\r\n"
                     "Server: basic-c-openssl\r\n"
                     "Content-Type: text/plain; charset=utf-8\r\n"
                     "Content-Length: %zu\r\n"
                     "Connection: close\r\n"
                     "\r\n"
                     "%s",
                     body_len, message);
    if (n < 0) {
        c->out_len = 0;
    } else if ((size_t)n >= sizeof(c->out_buf)) {
        c->out_len = sizeof(c->out_buf) - 1;
    } else {
        c->out_len = (size_t)n;
    }
    c->out_sent = 0;
    c->state = STATE_WRITING;
}

static void handle_http_request(struct connection *c) {
    struct http_request req;

    c->in_buf[c->in_len] = '\0';

    if (!http_parse_request(c->in_buf, &req)) {
        build_bad_request(c, "Bad HTTP request.\n");
        return;
    }

    http_handle_request(&req, c->out_buf, sizeof(c->out_buf), &c->out_len);
    c->out_sent = 0;
    c->state = STATE_WRITING;
}

static void drive_handshake(int epfd, struct connection *c) {
    int ret = SSL_accept(c->ssl);
    int err;

    if (ret == 1) {
        c->state = STATE_READING;
        update_interest(epfd, c, 0);
        return;
    }

    err = SSL_get_error(c->ssl, ret);
    if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
        update_interest(epfd, c, err);
        return;
    }

    close_connection(epfd, c);
}

static void drive_read(int epfd, struct connection *c) {
    int n;
    int err;

    while (1) {
        if (c->in_len + 1 >= sizeof(c->in_buf)) {
            build_bad_request(c, "Request header too large.\n");
            update_interest(epfd, c, 0);
            return;
        }

        n = SSL_read(c->ssl, c->in_buf + c->in_len,
                     (int)(sizeof(c->in_buf) - c->in_len - 1));
        if (n > 0) {
            c->in_len += (size_t)n;
            if (http_header_complete(c->in_buf, c->in_len)) {
                handle_http_request(c);
                update_interest(epfd, c, 0);
                return;
            }
            continue;
        }

        err = SSL_get_error(c->ssl, n);
        if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
            update_interest(epfd, c, err);
            return;
        }

        close_connection(epfd, c);
        return;
    }
}

static void drive_write(int epfd, struct connection *c) {
    int n;
    int err;

    while (c->out_sent < c->out_len) {
        n = SSL_write(c->ssl, c->out_buf + c->out_sent,
                      (int)(c->out_len - c->out_sent));
        if (n > 0) {
            c->out_sent += (size_t)n;
            continue;
        }

        err = SSL_get_error(c->ssl, n);
        if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
            update_interest(epfd, c, err);
            return;
        }

        close_connection(epfd, c);
        return;
    }

    close_connection(epfd, c);
}

static void accept_clients(int epfd, int listen_fd, SSL_CTX *ctx) {
    while (1) {
        int client_fd;
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        struct connection *c;

        client_fd = accept(listen_fd, (struct sockaddr *)&addr, &addr_len);
        if (client_fd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return;
            }
            perror("accept");
            return;
        }

        if (set_nonblocking(client_fd) < 0) {
            perror("set_nonblocking client fd");
            close(client_fd);
            continue;
        }

        c = (struct connection *)calloc(1, sizeof(*c));
        if (c == NULL) {
            close(client_fd);
            continue;
        }

        c->fd = client_fd;
        c->state = STATE_HANDSHAKE;
        c->ssl = SSL_new(ctx);
        if (c->ssl == NULL) {
            close_connection(epfd, c);
            continue;
        }

        SSL_set_fd(c->ssl, client_fd);
        SSL_set_accept_state(c->ssl);

        if (epoll_add_fd(epfd, client_fd, EPOLLIN | EPOLLOUT, c) < 0) {
            perror("epoll add client");
            close_connection(epfd, c);
            continue;
        }
    }
}

int main(int argc, char **argv) {
    int port = DEFAULT_PORT;
    int listen_fd;
    int epfd;
    struct epoll_event ev;
    struct epoll_event events[MAX_EVENTS];
    SSL_CTX *ssl_ctx;

    if (argc >= 2) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Invalid port: %s\n", argv[1]);
            return 1;
        }
    }

    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);

    ssl_global_init();
    session_init();

    ssl_ctx = ssl_create_server_ctx(CERT_FILE, KEY_FILE);
    if (ssl_ctx == NULL) {
        return 1;
    }

    listen_fd = create_listen_socket(port);
    if (listen_fd < 0) {
        SSL_CTX_free(ssl_ctx);
        return 1;
    }

    epfd = epoll_create1(0);
    if (epfd < 0) {
        perror("epoll_create1");
        close(listen_fd);
        SSL_CTX_free(ssl_ctx);
        return 1;
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.ptr = NULL;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) {
        perror("epoll add listen fd");
        close(epfd);
        close(listen_fd);
        SSL_CTX_free(ssl_ctx);
        return 1;
    }

    printf("HTTPS server listening on https://127.0.0.1:%d\n", port);
    printf("Use Ctrl+C to stop.\n");

    while (!g_stop) {
        int nready;
        int i;

        nready = epoll_wait(epfd, events, MAX_EVENTS, 1000);
        if (nready < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("epoll_wait");
            break;
        }

        session_cleanup_expired();

        for (i = 0; i < nready; i++) {
            struct connection *c = (struct connection *)events[i].data.ptr;

            if (c == NULL) {
                accept_clients(epfd, listen_fd, ssl_ctx);
                continue;
            }

            if ((events[i].events & (EPOLLERR | EPOLLHUP)) != 0) {
                close_connection(epfd, c);
                continue;
            }

            if (c->state == STATE_HANDSHAKE) {
                drive_handshake(epfd, c);
            } else if (c->state == STATE_READING) {
                drive_read(epfd, c);
            } else if (c->state == STATE_WRITING) {
                drive_write(epfd, c);
            } else {
                close_connection(epfd, c);
            }
        }
    }

    close(epfd);
    close(listen_fd);
    SSL_CTX_free(ssl_ctx);
    ssl_global_cleanup();
    printf("Server stopped.\n");
    return 0;
}
