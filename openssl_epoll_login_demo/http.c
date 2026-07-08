#include "http.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *find_header_end(const char *buf, size_t len) {
    for (size_t i = 0; i + 3 < len; ++i) {
        if (buf[i] == '\r' && buf[i + 1] == '\n' && buf[i + 2] == '\r' && buf[i + 3] == '\n') {
            return buf + i;
        }
    }
    return NULL;
}

static int starts_with_case(const char *s, const char *prefix) {
    while (*prefix) {
        if (tolower((unsigned char)*s) != tolower((unsigned char)*prefix)) return 0;
        ++s; ++prefix;
    }
    return 1;
}

static void trim_leading_spaces(const char **p) {
    while (**p == ' ' || **p == '\t') ++(*p);
}

static long parse_content_length(const char *headers) {
    const char *line = headers;
    while (*line) {
        const char *next = strstr(line, "\r\n");
        size_t line_len = next ? (size_t)(next - line) : strlen(line);
        if (line_len >= 15 && starts_with_case(line, "Content-Length:")) {
            const char *v = line + 15;
            trim_leading_spaces(&v);
            return strtol(v, NULL, 10);
        }
        if (!next) break;
        line = next + 2;
    }
    return 0;
}

static void parse_cookie_sid(const char *headers, char sid_out[SID_HEX_LEN + 1]) {
    sid_out[0] = '\0';
    const char *line = headers;
    while (*line) {
        const char *next = strstr(line, "\r\n");
        size_t line_len = next ? (size_t)(next - line) : strlen(line);
        if (line_len >= 7 && starts_with_case(line, "Cookie:")) {
            char cookie[MAX_COOKIE];
            size_t n = line_len - 7;
            if (n >= sizeof(cookie)) n = sizeof(cookie) - 1;
            memcpy(cookie, line + 7, n);
            cookie[n] = '\0';

            char *saveptr = NULL;
            for (char *tok = strtok_r(cookie, ";", &saveptr); tok; tok = strtok_r(NULL, ";", &saveptr)) {
                while (*tok == ' ' || *tok == '\t') ++tok;
                if (strncmp(tok, "SID=", 4) == 0) {
                    strncpy(sid_out, tok + 4, SID_HEX_LEN);
                    sid_out[SID_HEX_LEN] = '\0';
                    return;
                }
            }
        }
        if (!next) break;
        line = next + 2;
    }
}

int http_parse_request(const char *buf, size_t len, http_request *req) {
    memset(req, 0, sizeof(*req));
    const char *header_end = find_header_end(buf, len);
    if (!header_end) return 0;

    size_t header_len = (size_t)(header_end - buf);
    if (header_len >= 8192) return -1;

    char headers[8192];
    memcpy(headers, buf, header_len);
    headers[header_len] = '\0';

    char *line_end = strstr(headers, "\r\n");
    if (!line_end) return -1;
    *line_end = '\0';
    if (sscanf(headers, "%7s %255s", req->method, req->path) != 2) return -1;
    *line_end = '\r';

    long cl = parse_content_length(headers);
    if (cl < 0 || cl > MAX_BODY) return -1;
    size_t total_needed = header_len + 4 + (size_t)cl;
    if (len < total_needed) return 0;

    req->body_len = (size_t)cl;
    if (cl > 0) {
        memcpy(req->body, header_end + 4, (size_t)cl);
    }
    req->body[req->body_len] = '\0';
    parse_cookie_sid(headers, req->sid);
    return 1;
}

size_t http_build_response(char *out, size_t out_size, int status, const char *status_text,
                           const char *body, const char *extra_headers) {
    if (!body) body = "";
    if (!extra_headers) extra_headers = "";
    int n = snprintf(out, out_size,
        "HTTP/1.1 %d %s\r\n"
        "Server: epoll-openssl-demo\r\n"
        "Content-Type: text/plain; charset=utf-8\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "%s"
        "\r\n"
        "%s",
        status, status_text, strlen(body), extra_headers, body);
    if (n < 0) return 0;
    if ((size_t)n >= out_size) return out_size - 1;
    return (size_t)n;
}
