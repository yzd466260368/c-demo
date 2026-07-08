#ifndef SESSION_H
#define SESSION_H

#include <stddef.h>

void session_init(void);
int session_create(const char *username, char *sid_out, size_t sid_out_size);
const char *session_get_user(const char *sid);
void session_destroy(const char *sid);
void session_cleanup_expired(void);

#endif
