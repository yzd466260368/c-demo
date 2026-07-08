#include "util.h"

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void bytes_to_hex(const unsigned char *bytes, size_t n, char *out_hex) {
    static const char table[] = "0123456789abcdef";
    size_t i;

    for (i = 0; i < n; i++) {
        out_hex[i * 2] = table[(bytes[i] >> 4) & 0x0f];
        out_hex[i * 2 + 1] = table[bytes[i] & 0x0f];
    }
    out_hex[n * 2] = '\0';
}

static int hex_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

void url_decode(const char *src, char *dst, size_t dst_size) {
    size_t si = 0;
    size_t di = 0;

    if (dst_size == 0) {
        return;
    }

    while (src[si] != '\0' && di + 1 < dst_size) {
        if (src[si] == '%' && isxdigit((unsigned char)src[si + 1]) &&
            isxdigit((unsigned char)src[si + 2])) {
            int high = hex_value(src[si + 1]);
            int low = hex_value(src[si + 2]);
            dst[di++] = (char)((high << 4) | low);
            si += 3;
        } else if (src[si] == '+') {
            dst[di++] = ' ';
            si++;
        } else {
            dst[di++] = src[si++];
        }
    }

    dst[di] = '\0';
}
