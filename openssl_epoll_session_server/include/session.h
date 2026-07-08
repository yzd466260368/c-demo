#ifndef SESSION_H
#define SESSION_H

#include <stddef.h>

#define SID_HEX_LEN 64
#define USERNAME_LEN 63

int session_create(const char *username, char *sid_out, size_t sid_out_sz);
const char *session_validate(const char *sid);
void session_destroy(const char *sid);
void session_cleanup_expired(void);

#endif
