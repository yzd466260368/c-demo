#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

int set_nonblocking(int fd);
void bytes_to_hex(const unsigned char *bytes, size_t n, char *out_hex);
void url_decode(const char *src, char *dst, size_t dst_size);

#endif
