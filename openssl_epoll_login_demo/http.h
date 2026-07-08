#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>
#include "session.h"

#define MAX_METHOD 8
#define MAX_PATH 256
#define MAX_BODY 4096
#define MAX_COOKIE 1024
#define MAX_RESPONSE 8192

typedef struct {
    char method[MAX_METHOD];
    char path[MAX_PATH];
    char body[MAX_BODY + 1];
    size_t body_len;
    char sid[SID_HEX_LEN + 1];
} http_request;

/* return: 1 complete, 0 need more, -1 invalid */
int http_parse_request(const char *buf, size_t len, http_request *req);
size_t http_build_response(char *out, size_t out_size, int status, const char *status_text,
                           const char *body, const char *extra_headers);

#endif
