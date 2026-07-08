#define _GNU_SOURCE
#include "util.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void bytes_to_hex(const unsigned char *in, size_t in_len, char *out_hex) {
    static const char *hex = "0123456789abcdef";
    for (size_t i = 0; i < in_len; ++i) {
        out_hex[i * 2] = hex[(in[i] >> 4) & 0x0f];
        out_hex[i * 2 + 1] = hex[in[i] & 0x0f];
    }
    out_hex[in_len * 2] = '\0';
}

static int from_hex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

int url_decode(const char *src, char *dst, size_t dst_size) {
    size_t di = 0;
    for (size_t si = 0; src[si] != '\0'; ++si) {
        if (di + 1 >= dst_size) return -1;
        if (src[si] == '+') {
            dst[di++] = ' ';
        } else if (src[si] == '%' && isxdigit((unsigned char)src[si + 1]) && isxdigit((unsigned char)src[si + 2])) {
            int hi = from_hex(src[si + 1]);
            int lo = from_hex(src[si + 2]);
            dst[di++] = (char)((hi << 4) | lo);
            si += 2;
        } else {
            dst[di++] = src[si];
        }
    }
    dst[di] = '\0';
    return 0;
}

int form_get_value(const char *body, const char *key, char *out, size_t out_size) {
    size_t key_len = strlen(key);
    const char *p = body;
    while (*p) {
        const char *amp = strchr(p, '&');
        size_t pair_len = amp ? (size_t)(amp - p) : strlen(p);
        const char *eq = memchr(p, '=', pair_len);
        if (eq) {
            size_t klen = (size_t)(eq - p);
            if (klen == key_len && strncmp(p, key, key_len) == 0) {
                size_t vlen = pair_len - klen - 1;
                if (vlen >= 1024) return -1;
                char tmp[1024];
                memcpy(tmp, eq + 1, vlen);
                tmp[vlen] = '\0';
                return url_decode(tmp, out, out_size);
            }
        }
        if (!amp) break;
        p = amp + 1;
    }
    return -1;
}
