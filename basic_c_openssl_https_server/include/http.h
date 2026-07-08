#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>

struct http_request {
    char method[8];
    char path[256];
    char query[512];
    char cookie[512];
};

int http_header_complete(const char *buf, size_t len);
int http_parse_request(const char *raw, struct http_request *req);
void http_handle_request(const struct http_request *req, char *out, size_t out_size, size_t *out_len);

#endif
