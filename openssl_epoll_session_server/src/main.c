#include "http.h"
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

#define DEFAULT_PORT 8443
#define MAX_EVENTS 128
#define READ_BUF_SIZE 8192
#define WRITE_BUF_SIZE 16384

#ifndef CERT_FILE
#define CERT_FILE "cert/server.crt"
#endif

#ifndef KEY_FILE
#define KEY_FILE "cert/server.key"
#endif

enum conn_state {
    CONN_HANDSHAKE = 0,
    CONN_READING,
    CONN_WRITING
};

struct connection {
    int fd;
    SSL *ssl;
    enum conn_state state;
    char in_buf[READ_BUF_SIZE];
    size_t in_len;
    char out_buf[WRITE_BUF_SIZE];
    size_t out_len;
    size_t out_off;
};

static SSL_CTX *create_ssl_ctx(void) {
    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) return NULL;

    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);

    if (SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM) != 1) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return NULL;
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, KEY_FILE, SSL_FILETYPE_PEM) != 1) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return NULL;
    }
    if (SSL_CTX_check_private_key(ctx) != 1) {
        fprintf(stderr, "private key does not match certificate\n");
        SSL_CTX_free(ctx);
        return NULL;
    }
    return ctx;
}

static int create_listen_socket(int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("setsockopt SO_REUSEADDR");
        close(fd);
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((uint16_t)port);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }
    if (listen(fd, SOMAXCONN) < 0) {
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

static void epoll_mod_conn(int epfd, struct connection *c, uint32_t events) {
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = events;
    ev.data.ptr = c;
    if (epoll_ctl(epfd, EPOLL_CTL_MOD, c->fd, &ev) < 0) {
        perror("epoll_ctl MOD");
    }
}

static void close_connection(int epfd, struct connection *c) {
    if (!c) return;
    epoll_ctl(epfd, EPOLL_CTL_DEL, c->fd, NULL);
    if (c->ssl) {
        SSL_shutdown(c->ssl);
        SSL_free(c->ssl);
    }
    close(c->fd);
    free(c);
}

static void handle_ssl_error_for_io(int epfd, struct connection *c, int ssl_ret, int for_write) {
    int err = SSL_get_error(c->ssl, ssl_ret);
    if (err == SSL_ERROR_WANT_READ) {
        epoll_mod_conn(epfd, c, EPOLLIN);
    } else if (err == SSL_ERROR_WANT_WRITE) {
        epoll_mod_conn(epfd, c, EPOLLOUT);
    } else if (err == SSL_ERROR_ZERO_RETURN) {
        close_connection(epfd, c);
    } else {
        fprintf(stderr, "%s failed: SSL error %d\n", for_write ? "SSL_write" : "SSL_read", err);
        ERR_print_errors_fp(stderr);
        close_connection(epfd, c);
    }
}

static void do_handshake(int epfd, struct connection *c) {
    int ret = SSL_accept(c->ssl);
    if (ret == 1) {
        c->state = CONN_READING;
        epoll_mod_conn(epfd, c, EPOLLIN);
        return;
    }

    int err = SSL_get_error(c->ssl, ret);
    if (err == SSL_ERROR_WANT_READ) {
        epoll_mod_conn(epfd, c, EPOLLIN);
    } else if (err == SSL_ERROR_WANT_WRITE) {
        epoll_mod_conn(epfd, c, EPOLLOUT);
    } else {
        fprintf(stderr, "SSL_accept failed: SSL error %d\n", err);
        ERR_print_errors_fp(stderr);
        close_connection(epfd, c);
    }
}

static void do_read(int epfd, struct connection *c) {
    while (c->in_len + 1 < sizeof(c->in_buf)) {
        int n = SSL_read(c->ssl, c->in_buf + c->in_len, (int)(sizeof(c->in_buf) - c->in_len - 1));
        if (n > 0) {
            c->in_len += (size_t)n;
            c->in_buf[c->in_len] = '\0';
            if (strstr(c->in_buf, "\r\n\r\n") != NULL) {
                c->out_len = http_handle_request(c->in_buf, c->out_buf, sizeof(c->out_buf));
                c->out_off = 0;
                c->state = CONN_WRITING;
                epoll_mod_conn(epfd, c, EPOLLOUT);
                return;
            }
            continue;
        }
        handle_ssl_error_for_io(epfd, c, n, 0);
        return;
    }

    const char *too_large =
        "HTTP/1.1 413 Payload Too Large\r\n"
        "Content-Type: text/plain; charset=utf-8\r\n"
        "Content-Length: 18\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Request too large\n";
    snprintf(c->out_buf, sizeof(c->out_buf), "%s", too_large);
    c->out_len = strlen(c->out_buf);
    c->out_off = 0;
    c->state = CONN_WRITING;
    epoll_mod_conn(epfd, c, EPOLLOUT);
}

static void do_write(int epfd, struct connection *c) {
    while (c->out_off < c->out_len) {
        int n = SSL_write(c->ssl, c->out_buf + c->out_off, (int)(c->out_len - c->out_off));
        if (n > 0) {
            c->out_off += (size_t)n;
            continue;
        }
        handle_ssl_error_for_io(epfd, c, n, 1);
        return;
    }
    close_connection(epfd, c);
}

static void accept_loop(int epfd, int listen_fd, SSL_CTX *ctx) {
    for (;;) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return;
            if (errno == EINTR) continue;
            perror("accept");
            return;
        }

        if (set_nonblocking(client_fd) < 0) {
            perror("set_nonblocking client fd");
            close(client_fd);
            continue;
        }

        struct connection *c = calloc(1, sizeof(*c));
        if (!c) {
            perror("calloc connection");
            close(client_fd);
            continue;
        }
        c->fd = client_fd;
        c->ssl = SSL_new(ctx);
        if (!c->ssl) {
            ERR_print_errors_fp(stderr);
            close(client_fd);
            free(c);
            continue;
        }
        SSL_set_fd(c->ssl, client_fd);
        SSL_set_accept_state(c->ssl);
        c->state = CONN_HANDSHAKE;

        struct epoll_event ev;
        memset(&ev, 0, sizeof(ev));
        ev.events = EPOLLIN;
        ev.data.ptr = c;
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
            perror("epoll_ctl ADD client");
            close_connection(epfd, c);
            continue;
        }
    }
}

int main(int argc, char **argv) {
    int port = DEFAULT_PORT;
    if (argc >= 2) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "usage: %s [port]\n", argv[0]);
            return 1;
        }
    }

    signal(SIGPIPE, SIG_IGN);

    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    SSL_CTX *ctx = create_ssl_ctx();
    if (!ctx) {
        fprintf(stderr, "failed to create SSL_CTX. Did you run: make cert ?\n");
        return 1;
    }

    int listen_fd = create_listen_socket(port);
    if (listen_fd < 0) {
        SSL_CTX_free(ctx);
        return 1;
    }

    int epfd = epoll_create1(0);
    if (epfd < 0) {
        perror("epoll_create1");
        close(listen_fd);
        SSL_CTX_free(ctx);
        return 1;
    }

    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.ptr = NULL; /* NULL means listening socket event */
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) {
        perror("epoll_ctl ADD listen");
        close(epfd);
        close(listen_fd);
        SSL_CTX_free(ctx);
        return 1;
    }

    printf("HTTPS server listening on https://127.0.0.1:%d\n", port);
    printf("Try: curl -k -i 'https://127.0.0.1:%d/login?user=alice&password=123456' -c cookie.txt\n", port);

    struct epoll_event events[MAX_EVENTS];
    for (;;) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < n; ++i) {
            if (events[i].data.ptr == NULL) {
                accept_loop(epfd, listen_fd, ctx);
                continue;
            }

            struct connection *c = (struct connection *)events[i].data.ptr;
            if (events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                close_connection(epfd, c);
                continue;
            }

            if (c->state == CONN_HANDSHAKE) {
                do_handshake(epfd, c);
            } else if (c->state == CONN_READING && (events[i].events & EPOLLIN)) {
                do_read(epfd, c);
            } else if (c->state == CONN_WRITING && (events[i].events & (EPOLLIN | EPOLLOUT))) {
                do_write(epfd, c);
            }
        }
    }

    close(epfd);
    close(listen_fd);
    SSL_CTX_free(ctx);
    EVP_cleanup();
    return 0;
}
