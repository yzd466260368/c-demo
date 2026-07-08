#ifndef SESSION_H
#define SESSION_H

#include <stddef.h>

#define SID_BYTES 32
#define SID_HEX_LEN (SID_BYTES * 2)

int session_create(const char *username, char sid_out[SID_HEX_LEN + 1]);
const char *session_lookup(const char *sid);
int session_destroy(const char *sid);

#endif
