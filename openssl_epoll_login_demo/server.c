#define _GNU_SOURCE
#include "server.h"
#include "auth.h"
#include "http.h"
#include "session.h"
#include "util.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EVENTS 64
#define MAX_REQUEST 16384

typedef enum
{
    CONN_HANDSHAKE = 0,
    CONN_READING,
    CONN_WRITING,
    CONN_CLOSING
} conn_state;

typedef struct connection
{
    int fd;
    SSL *ssl;
    conn_state state;
    int want_read;
    int want_write;
    char inbuf[MAX_REQUEST];
    size_t in_len;
    char outbuf[MAX_RESPONSE];
    size_t out_len;
    size_t out_sent;
} connection;

static int create_listen_socket(int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return -1;

    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((uint16_t)port);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(fd);
        return -1;
    }
    if (listen(fd, SOMAXCONN) < 0)
    {
        close(fd);
        return -1;
    }
    if (set_nonblocking(fd) < 0)
    {
        close(fd);
        return -1;
    }
    return fd;
}

static void update_conn_events(int epfd, connection *c)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.ptr = c;

    if (c->state == CONN_HANDSHAKE)
    {
        if (c->want_read)
            ev.events |= EPOLLIN;
        if (c->want_write)
            ev.events |= EPOLLOUT;
    }
    else if (c->state == CONN_READING)
    {
        ev.events |= EPOLLIN;
        if (c->want_write)
            ev.events |= EPOLLOUT;
    }
    else if (c->state == CONN_WRITING)
    {
        ev.events |= EPOLLOUT;
        if (c->want_read)
            ev.events |= EPOLLIN;
    }

    if (ev.events == 0)
        ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_MOD, c->fd, &ev);
}

static void close_connection(int epfd, connection *c)
{
    if (!c)
        return;
    epoll_ctl(epfd, EPOLL_CTL_DEL, c->fd, NULL);
    if (c->ssl)
    {
        SSL_shutdown(c->ssl);
        SSL_free(c->ssl);
    }
    close(c->fd);
    free(c);
}

static void set_response(connection *c, int status, const char *status_text, const char *body, const char *extra_headers)
{
    c->out_len = http_build_response(c->outbuf, sizeof(c->outbuf), status, status_text, body, extra_headers);
    c->out_sent = 0;
    c->state = CONN_WRITING;
    c->want_read = 0;
    c->want_write = 1;
}

static void route_request(connection *c, const http_request *req)
{
    if (strcmp(req->method, "POST") == 0 && strcmp(req->path, "/login") == 0)
    {
        char username[128], password[128];
        if (form_get_value(req->body, "username", username, sizeof(username)) != 0 ||
            form_get_value(req->body, "password", password, sizeof(password)) != 0)
        {
            set_response(c, 400, "Bad Request", "need form fields: username and password\n", NULL);
            return;
        }
        if (!auth_verify(username, password))
        {
            set_response(c, 401, "Unauthorized", "login failed\n", NULL);
            return;
        }
        char sid[SID_HEX_LEN + 1];
        if (session_create(username, sid) != 0)
        {
            set_response(c, 500, "Internal Server Error", "cannot create session\n", NULL);
            return;
        }
        char header[256];
        snprintf(header, sizeof(header),
                 "Set-Cookie: SID=%s; Path=/; HttpOnly; SameSite=Lax; Secure\r\n", sid);
        set_response(c, 200, "OK", "login ok\n", header);
        return;
    }

    if (strcmp(req->method, "GET") == 0 && strcmp(req->path, "/me") == 0)
    {
        const char *username = session_lookup(req->sid);
        if (!username)
        {
            set_response(c, 401, "Unauthorized", "not logged in\n", NULL);
            return;
        }
        char body[256];
        snprintf(body, sizeof(body), "current user: %s\n", username);
        set_response(c, 200, "OK", body, NULL);
        return;
    }

    if ((strcmp(req->method, "POST") == 0 || strcmp(req->method, "GET") == 0) &&
        strcmp(req->path, "/logout") == 0)
    {
        if (req->sid[0])
            session_destroy(req->sid);
        set_response(c, 200, "OK", "logout ok\n",
                     "Set-Cookie: SID=deleted; Path=/; Max-Age=0; HttpOnly; SameSite=Lax; Secure\r\n");
        return;
    }

    set_response(c, 404, "Not Found",
                 "not found\ntry: POST /login, GET /me, POST /logout\n", NULL);
}

static int do_handshake(int epfd, connection *c)
{
    int ret = SSL_accept(c->ssl);
    if (ret == 1)
    {
        c->state = CONN_READING;
        c->want_read = 1;
        c->want_write = 0;
        update_conn_events(epfd, c);
        return 0;
    }

    int err = SSL_get_error(c->ssl, ret);
    c->want_read = (err == SSL_ERROR_WANT_READ);
    c->want_write = (err == SSL_ERROR_WANT_WRITE);
    if (c->want_read || c->want_write)
    {
        update_conn_events(epfd, c);
        return 0;
    }
    return -1;
}

static int do_read(int epfd, connection *c)
{
    for (;;)
    {
        if (c->in_len >= sizeof(c->inbuf))
        {
            set_response(c, 413, "Payload Too Large", "request too large\n", NULL);
            update_conn_events(epfd, c);
            return 0;
        }

        int n = SSL_read(c->ssl, c->inbuf + c->in_len, (int)(sizeof(c->inbuf) - c->in_len));
        if (n > 0)
        {
            c->in_len += (size_t)n;
            http_request req;
            int pr = http_parse_request(c->inbuf, c->in_len, &req);
            if (pr == 1)
            {
                route_request(c, &req);
                update_conn_events(epfd, c);
                return 0;
            }
            else if (pr < 0)
            {
                set_response(c, 400, "Bad Request", "bad request\n", NULL);
                update_conn_events(epfd, c);
                return 0;
            }
            continue;
        }

        int err = SSL_get_error(c->ssl, n);
        if (err == SSL_ERROR_WANT_READ)
        {
            c->want_read = 1;
            c->want_write = 0;
            update_conn_events(epfd, c);
            return 0;
        }
        if (err == SSL_ERROR_WANT_WRITE)
        {
            c->want_read = 0;
            c->want_write = 1;
            update_conn_events(epfd, c);
            return 0;
        }
        return -1;
    }
}

static int do_write(int epfd, connection *c)
{
    while (c->out_sent < c->out_len)
    {
        int n = SSL_write(c->ssl, c->outbuf + c->out_sent, (int)(c->out_len - c->out_sent));
        if (n > 0)
        {
            c->out_sent += (size_t)n;
            continue;
        }
        int err = SSL_get_error(c->ssl, n);
        if (err == SSL_ERROR_WANT_WRITE)
        {
            c->want_write = 1;
            c->want_read = 0;
            update_conn_events(epfd, c);
            return 0;
        }
        if (err == SSL_ERROR_WANT_READ)
        {
            c->want_read = 1;
            c->want_write = 0;
            update_conn_events(epfd, c);
            return 0;
        }
        return -1;
    }
    c->state = CONN_CLOSING;
    return 1;
}

static void accept_new_connections(int epfd, int listen_fd, SSL_CTX *ctx)
{
    for (;;)
    {
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        int fd = accept(listen_fd, (struct sockaddr *)&addr, &addr_len);
        if (fd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return;
            perror("accept");
            return;
        }
        if (set_nonblocking(fd) < 0)
        {
            close(fd);
            continue;
        }

        connection *c = calloc(1, sizeof(*c));
        if (!c)
        {
            close(fd);
            continue;
        }
        c->fd = fd;
        c->ssl = SSL_new(ctx);
        if (!c->ssl)
        {
            close(fd);
            free(c);
            continue;
        }
        SSL_set_fd(c->ssl, fd);
        SSL_set_accept_state(c->ssl);
        c->state = CONN_HANDSHAKE;
        c->want_read = 1;
        c->want_write = 0;

        struct epoll_event ev;
        memset(&ev, 0, sizeof(ev));
        ev.events = EPOLLIN;
        ev.data.ptr = c;
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) < 0)
        {
            SSL_free(c->ssl);
            close(fd);
            free(c);
        }
    }
}

int server_run(int port, SSL_CTX *ctx)
{
    int listen_fd = create_listen_socket(port);
    if (listen_fd < 0)
        die("create_listen_socket");

    int epfd = epoll_create1(0);
    if (epfd < 0)
        die("epoll_create1");

    struct epoll_event lev;
    memset(&lev, 0, sizeof(lev));
    lev.events = EPOLLIN;
    lev.data.fd = listen_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &lev) < 0)
        die("epoll_ctl listen");

    printf("HTTPS server listening on https://127.0.0.1:%d\n", port);
    printf("Demo users: alice/123456, bob/password\n");

    struct epoll_event events[MAX_EVENTS];
    for (;;)
    {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            die("epoll_wait");
        }
        for (int i = 0; i < n; ++i)
        {
            if (events[i].data.fd == listen_fd)
            {
                accept_new_connections(epfd, listen_fd, ctx);
                continue;
            }

            connection *c = (connection *)events[i].data.ptr;
            int close_now = 0;
            if (events[i].events & (EPOLLERR | EPOLLHUP))
            {
                close_now = 1;
            }
            else if (c->state == CONN_HANDSHAKE)
            {
                if (do_handshake(epfd, c) < 0)
                    close_now = 1;
            }
            else if (c->state == CONN_READING)
            {
                if (do_read(epfd, c) < 0)
                    close_now = 1;
            }
            else if (c->state == CONN_WRITING)
            {
                int wr = do_write(epfd, c);
                if (wr != 0)
                    close_now = 1;
            }
            else
            {
                close_now = 1;
            }

            if (close_now)
                close_connection(epfd, c);
        }
    }

    close(listen_fd);
    close(epfd);
    return 0;
}
