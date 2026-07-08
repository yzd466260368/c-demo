#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>

size_t http_handle_request(const char *request, char *response, size_t response_size);

#endif
