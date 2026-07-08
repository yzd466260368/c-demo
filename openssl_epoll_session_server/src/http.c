#include "http.h"
#include "session.h"
#include "user.h"
#include "util.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <time.h>

#define METHOD_LEN 15
#define URI_LEN 1023
#define PATH_LEN 511
#define QUERY_LEN 1023
#define COOKIE_LEN 2047
#define VALUE_LEN 255

struct request_info {
    char method[METHOD_LEN + 1];
    char uri[URI_LEN + 1];
    char path[PATH_LEN + 1];
    char query[QUERY_LEN + 1];
    char cookie[COOKIE_LEN + 1];
};

static size_t appendf(char *buf, size_t cap, size_t off, const char *fmt, ...) {
    if (off >= cap) return off;
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf + off, cap - off, fmt, ap);
    va_end(ap);
    if (n < 0) return off;
    size_t nn = (size_t)n;
    if (nn >= cap - off) return cap - 1;
    return off + nn;
}

static size_t build_response(char *out, size_t out_sz, int code, const char *reason,
                             const char *extra_headers, const char *body) {
    size_t body_len = body ? strlen(body) : 0;
    size_t off = 0;
    off = appendf(out, out_sz, off,
                  "HTTP/1.1 %d %s\r\n"
                  "Server: c-epoll-openssl-demo\r\n"
                  "Content-Type: text/plain; charset=utf-8\r\n"
                  "Content-Length: %zu\r\n"
                  "Connection: close\r\n",
                  code, reason, body_len);
    if (extra_headers && extra_headers[0] != '\0') {
        off = appendf(out, out_sz, off, "%s", extra_headers);
    }
    off = appendf(out, out_sz, off, "\r\n");
    if (body_len > 0) {
        off = appendf(out, out_sz, off, "%s", body);
    }
    return off;
}

static int parse_request_line(const char *req, struct request_info *ri) {
    memset(ri, 0, sizeof(*ri));
    if (sscanf(req, "%15s %1023s", ri->method, ri->uri) != 2) return 0;

    const char *q = strchr(ri->uri, '?');
    if (q) {
        size_t path_len = (size_t)(q - ri->uri);
        if (path_len >= sizeof(ri->path)) path_len = sizeof(ri->path) - 1;
        memcpy(ri->path, ri->uri, path_len);
        ri->path[path_len] = '\0';
        snprintf(ri->query, sizeof(ri->query), "%s", q + 1);
    } else {
        size_t path_len = strlen(ri->uri);
        if (path_len >= sizeof(ri->path)) path_len = sizeof(ri->path) - 1;
        memcpy(ri->path, ri->uri, path_len);
        ri->path[path_len] = '\0';
        ri->query[0] = '\0';
    }
    return 1;
}

static void read_cookie_header(const char *req, char *cookie_out, size_t cookie_sz) {
    cookie_out[0] = '\0';
    const char *line = strstr(req, "\r\n");
    if (!line) return;
    line += 2;

    while (*line && strncmp(line, "\r\n", 2) != 0) {
        const char *end = strstr(line, "\r\n");
        if (!end) break;
        size_t line_len = (size_t)(end - line);
        if (line_len >= 7 && strncasecmp(line, "Cookie:", 7) == 0) {
            const char *v = line + 7;
            while (*v == ' ' || *v == '\t') ++v;
            size_t v_len = (size_t)(end - v);
            if (v_len >= cookie_sz) v_len = cookie_sz - 1;
            memcpy(cookie_out, v, v_len);
            cookie_out[v_len] = '\0';
            return;
        }
        line = end + 2;
    }
}

static int query_get(const char *query, const char *key, char *value_out, size_t value_sz) {
    value_out[0] = '\0';
    if (!query || !key) return 0;

    size_t key_len = strlen(key);
    const char *p = query;
    while (*p) {
        const char *amp = strchr(p, '&');
        size_t pair_len = amp ? (size_t)(amp - p) : strlen(p);
        const char *eq = memchr(p, '=', pair_len);
        if (eq) {
            size_t k_len = (size_t)(eq - p);
            if (k_len == key_len && strncmp(p, key, key_len) == 0) {
                size_t raw_len = pair_len - k_len - 1;
                char raw[VALUE_LEN * 3 + 1];
                if (raw_len >= sizeof(raw)) raw_len = sizeof(raw) - 1;
                memcpy(raw, eq + 1, raw_len);
                raw[raw_len] = '\0';
                return url_decode(raw, value_out, value_sz) == 0;
            }
        }
        if (!amp) break;
        p = amp + 1;
    }
    return 0;
}

static int cookie_get_sid(const char *cookie, char *sid_out, size_t sid_sz) {
    sid_out[0] = '\0';
    if (!cookie) return 0;

    const char *p = cookie;
    while (*p) {
        while (*p == ' ' || *p == '\t' || *p == ';') ++p;
        const char *end = strchr(p, ';');
        size_t part_len = end ? (size_t)(end - p) : strlen(p);
        const char *eq = memchr(p, '=', part_len);
        if (eq) {
            size_t k_len = (size_t)(eq - p);
            if (k_len == 3 && strncmp(p, "SID", 3) == 0) {
                size_t v_len = part_len - k_len - 1;
                if (v_len >= sid_sz) v_len = sid_sz - 1;
                memcpy(sid_out, eq + 1, v_len);
                sid_out[v_len] = '\0';
                return 1;
            }
        }
        if (!end) break;
        p = end + 1;
    }
    return 0;
}

size_t http_handle_request(const char *request, char *response, size_t response_size) {
    struct request_info ri;
    if (!parse_request_line(request, &ri)) {
        return build_response(response, response_size, 400, "Bad Request", NULL,
                              "Bad Request\n");
    }
    read_cookie_header(request, ri.cookie, sizeof(ri.cookie));

    if (strcmp(ri.path, "/") == 0) {
        const char *body =
            "C epoll + OpenSSL session demo\n\n"
            "Endpoints:\n"
            "  GET /login?user=alice&password=123456\n"
            "  GET /me\n"
            "  GET /logout\n\n"
            "Demo users:\n"
            "  alice / 123456\n"
            "  bob   / password\n";
        return build_response(response, response_size, 200, "OK", NULL, body);
    }

    if (strcmp(ri.path, "/login") == 0) {
        char username[VALUE_LEN + 1];
        char password[VALUE_LEN + 1];
        if (!query_get(ri.query, "user", username, sizeof(username)) ||
            !query_get(ri.query, "password", password, sizeof(password))) {
            return build_response(response, response_size, 400, "Bad Request", NULL,
                                  "Usage: /login?user=alice&password=123456\n");
        }

        if (!user_verify_password(username, password)) {
            return build_response(response, response_size, 401, "Unauthorized", NULL,
                                  "Login failed\n");
        }

        char sid[SID_HEX_LEN + 1];
        if (!session_create(username, sid, sizeof(sid))) {
            return build_response(response, response_size, 500, "Internal Server Error", NULL,
                                  "Cannot create session\n");
        }

        char headers[256];
        snprintf(headers, sizeof(headers),
                 "Set-Cookie: SID=%s; Path=/; HttpOnly; Secure; SameSite=Lax; Max-Age=3600\r\n",
                 sid);
        char body[256];
        snprintf(body, sizeof(body), "Login success. user=%s\n", username);
        return build_response(response, response_size, 200, "OK", headers, body);
    }

    if (strcmp(ri.path, "/me") == 0) {
        char sid[SID_HEX_LEN + 1];
        if (!cookie_get_sid(ri.cookie, sid, sizeof(sid))) {
            return build_response(response, response_size, 401, "Unauthorized", NULL,
                                  "No SID cookie. Please login first.\n");
        }
        const char *username = session_validate(sid);
        if (!username) {
            return build_response(response, response_size, 401, "Unauthorized", NULL,
                                  "Invalid or expired session.\n");
        }
        char body[256];
        snprintf(body, sizeof(body), "You are logged in as %s\n", username);
        return build_response(response, response_size, 200, "OK", NULL, body);
    }

    if (strcmp(ri.path, "/logout") == 0) {
        char sid[SID_HEX_LEN + 1];
        if (cookie_get_sid(ri.cookie, sid, sizeof(sid))) {
            session_destroy(sid);
        }
        const char *headers =
            "Set-Cookie: SID=deleted; Path=/; HttpOnly; Secure; SameSite=Lax; Max-Age=0\r\n";
        return build_response(response, response_size, 200, "OK", headers,
                              "Logout success\n");
    }

    return build_response(response, response_size, 404, "Not Found", NULL,
                          "Not Found\n");
}
