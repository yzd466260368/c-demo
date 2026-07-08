#include "util.h"

#include <ctype.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void bytes_to_hex(const unsigned char *bytes, size_t len, char *hex_out, size_t hex_out_sz) {
    static const char table[] = "0123456789abcdef";
    if (hex_out_sz < len * 2 + 1) {
        if (hex_out_sz > 0) hex_out[0] = '\0';
        return;
    }
    for (size_t i = 0; i < len; ++i) {
        hex_out[i * 2] = table[(bytes[i] >> 4) & 0x0f];
        hex_out[i * 2 + 1] = table[bytes[i] & 0x0f];
    }
    hex_out[len * 2] = '\0';
}

static int hex_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

int url_decode(const char *src, char *dst, size_t dst_sz) {
    size_t di = 0;
    for (size_t si = 0; src[si] != '\0'; ++si) {
        if (di + 1 >= dst_sz) return -1;
        if (src[si] == '%' && isxdigit((unsigned char)src[si + 1]) && isxdigit((unsigned char)src[si + 2])) {
            int hi = hex_value(src[si + 1]);
            int lo = hex_value(src[si + 2]);
            dst[di++] = (char)((hi << 4) | lo);
            si += 2;
        } else if (src[si] == '+') {
            dst[di++] = ' ';
        } else {
            dst[di++] = src[si];
        }
    }
    dst[di] = '\0';
    return 0;
}
