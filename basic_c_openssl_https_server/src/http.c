#include "http.h"
#include "session.h"
#include "user.h"
#include "util.h"

#include <stdio.h>
#include <string.h>

static void copy_range(char *dst, size_t dst_size, const char *begin, const char *end) {
    size_t n;

    if (dst_size == 0) {
        return;
    }

    if (end < begin) {
        dst[0] = '\0';
        return;
    }

    n = (size_t)(end - begin);
    if (n >= dst_size) {
        n = dst_size - 1;
    }

    memcpy(dst, begin, n);
    dst[n] = '\0';
}

int http_header_complete(const char *buf, size_t len) {
    size_t i;

    if (buf == NULL || len < 4) {
        return 0;
    }

    for (i = 0; i + 3 < len; i++) {
        if (buf[i] == '\r' && buf[i + 1] == '\n' &&
            buf[i + 2] == '\r' && buf[i + 3] == '\n') {
            return 1;
        }
    }

    return 0;
}

static void parse_cookie_header(const char *raw, char *cookie_out, size_t cookie_size) {
    const char *p = raw;
    const char *line_end;

    cookie_out[0] = '\0';

    while ((p = strstr(p, "\r\n")) != NULL) {
        p += 2;
        if (*p == '\r' && *(p + 1) == '\n') {
            break;
        }
        line_end = strstr(p, "\r\n");
        if (line_end == NULL) {
            break;
        }
        if (strncmp(p, "Cookie:", 7) == 0) {
            p += 7;
            while (*p == ' ') p++;
            copy_range(cookie_out, cookie_size, p, line_end);
            return;
        }
    }
}

int http_parse_request(const char *raw, struct http_request *req) {
    const char *line_end;
    char first_line[768];
    char target[512];
    char decoded_path[256];
    char *q;

    if (raw == NULL || req == NULL) {
        return 0;
    }

    memset(req, 0, sizeof(*req));

    line_end = strstr(raw, "\r\n");
    if (line_end == NULL) {
        return 0;
    }

    copy_range(first_line, sizeof(first_line), raw, line_end);

    if (sscanf(first_line, "%7s %511s", req->method, target) != 2) {
        return 0;
    }

    q = strchr(target, '?');
    if (q != NULL) {
        *q = '\0';
        strncpy(req->query, q + 1, sizeof(req->query) - 1);
    }

    url_decode(target, decoded_path, sizeof(decoded_path));
    snprintf(req->path, sizeof(req->path), "%s", decoded_path);

    parse_cookie_header(raw, req->cookie, sizeof(req->cookie));
    return 1;
}

static int query_param(const char *query, const char *key, char *out, size_t out_size) {
    const char *p = query;
    size_t key_len = strlen(key);
    char tmp[512];
    const char *end;
    const char *eq;

    if (out_size == 0) {
        return 0;
    }
    out[0] = '\0';

    while (p != NULL && *p != '\0') {
        end = strchr(p, '&');
        if (end == NULL) {
            end = p + strlen(p);
        }

        eq = memchr(p, '=', (size_t)(end - p));
        if (eq != NULL && (size_t)(eq - p) == key_len && strncmp(p, key, key_len) == 0) {
            copy_range(tmp, sizeof(tmp), eq + 1, end);
            url_decode(tmp, out, out_size);
            return 1;
        }

        p = (*end == '&') ? end + 1 : end;
    }

    return 0;
}

static int cookie_sid(const char *cookie, char *sid_out, size_t sid_size) {
    const char *p = cookie;
    const char *end;

    if (sid_size == 0) {
        return 0;
    }
    sid_out[0] = '\0';

    while (p != NULL && *p != '\0') {
        while (*p == ' ' || *p == ';') p++;
        if (strncmp(p, "SID=", 4) == 0) {
            p += 4;
            end = strchr(p, ';');
            if (end == NULL) {
                end = p + strlen(p);
            }
            copy_range(sid_out, sid_size, p, end);
            return 1;
        }
        p = strchr(p, ';');
        if (p != NULL) p++;
    }

    return 0;
}

static void make_response(char *out, size_t out_size, size_t *out_len,
                          int status, const char *reason,
                          const char *extra_headers,
                          const char *body) {
    int n;
    size_t body_len = strlen(body);

    if (extra_headers == NULL) {
        extra_headers = "";
    }

    n = snprintf(out, out_size,
                 "HTTP/1.1 %d %s\r\n"
                 "Server: basic-c-openssl\r\n"
                 "Content-Type: text/plain; charset=utf-8\r\n"
                 "Content-Length: %zu\r\n"
                 "Connection: close\r\n"
                 "%s"
                 "\r\n"
                 "%s",
                 status, reason, body_len, extra_headers, body);

    if (n < 0) {
        *out_len = 0;
    } else if ((size_t)n >= out_size) {
        *out_len = out_size - 1;
    } else {
        *out_len = (size_t)n;
    }
}

void http_handle_request(const struct http_request *req, char *out, size_t out_size, size_t *out_len) {
    char username[64];
    char password[64];
    char sid[65];
    char header[256];
    const char *login_user;

    if (req == NULL || out == NULL || out_len == NULL) {
        return;
    }

    if (strcmp(req->method, "GET") != 0) {
        make_response(out, out_size, out_len, 405, "Method Not Allowed", NULL,
                      "Only GET is supported in this demo.\n");
        return;
    }

    if (strcmp(req->path, "/") == 0) {
        make_response(out, out_size, out_len, 200, "OK", NULL,
                      "Basic C + OpenSSL HTTPS login demo\n\n"
                      "Try:\n"
                      "  /login?user=alice&password=123456\n"
                      "  /me\n"
                      "  /logout\n");
        return;
    }

    if (strcmp(req->path, "/login") == 0) {
        if (!query_param(req->query, "user", username, sizeof(username)) ||
            !query_param(req->query, "password", password, sizeof(password))) {
            make_response(out, out_size, out_len, 400, "Bad Request", NULL,
                          "Missing user or password.\n");
            return;
        }

        if (!user_check_password(username, password)) {
            make_response(out, out_size, out_len, 401, "Unauthorized", NULL,
                          "Login failed.\n");
            return;
        }

        if (!session_create(username, sid, sizeof(sid))) {
            make_response(out, out_size, out_len, 500, "Internal Server Error", NULL,
                          "Cannot create session.\n");
            return;
        }

        snprintf(header, sizeof(header),
                 "Set-Cookie: SID=%s; Path=/; HttpOnly; Secure; SameSite=Lax\r\n",
                 sid);
        make_response(out, out_size, out_len, 200, "OK", header,
                      "Login success. Cookie SID has been set.\n");
        return;
    }

    if (strcmp(req->path, "/me") == 0) {
        if (!cookie_sid(req->cookie, sid, sizeof(sid))) {
            make_response(out, out_size, out_len, 401, "Unauthorized", NULL,
                          "No SID cookie. Please login first.\n");
            return;
        }

        login_user = session_get_user(sid);
        if (login_user == NULL) {
            make_response(out, out_size, out_len, 401, "Unauthorized", NULL,
                          "Invalid or expired session.\n");
            return;
        }

        snprintf(header, sizeof(header), "You are logged in as: %s\n", login_user);
        make_response(out, out_size, out_len, 200, "OK", NULL, header);
        return;
    }

    if (strcmp(req->path, "/logout") == 0) {
        if (cookie_sid(req->cookie, sid, sizeof(sid))) {
            session_destroy(sid);
        }
        make_response(out, out_size, out_len, 200, "OK",
                      "Set-Cookie: SID=deleted; Path=/; Max-Age=0; HttpOnly; Secure; SameSite=Lax\r\n",
                      "Logout success. Session is destroyed.\n");
        return;
    }

    make_response(out, out_size, out_len, 404, "Not Found", NULL,
                  "Not found.\n");
}
