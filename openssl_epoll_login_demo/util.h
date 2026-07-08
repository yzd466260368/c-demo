#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

int set_nonblocking(int fd);
void die(const char *msg);
void bytes_to_hex(const unsigned char *in, size_t in_len, char *out_hex);
int url_decode(const char *src, char *dst, size_t dst_size);
int form_get_value(const char *body, const char *key, char *out, size_t out_size);

#endif
