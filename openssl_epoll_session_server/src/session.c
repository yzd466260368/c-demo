#include "session.h"
#include "util.h"

#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SESSIONS 1024
#define SESSION_TTL_SECONDS 3600

struct session_entry {
    int used;
    char sid[SID_HEX_LEN + 1];
    char username[USERNAME_LEN + 1];
    time_t expires_at;
};

static struct session_entry sessions[MAX_SESSIONS];

void session_cleanup_expired(void) {
    time_t now = time(NULL);
    for (size_t i = 0; i < MAX_SESSIONS; ++i) {
        if (sessions[i].used && sessions[i].expires_at <= now) {
            memset(&sessions[i], 0, sizeof(sessions[i]));
        }
    }
}

int session_create(const char *username, char *sid_out, size_t sid_out_sz) {
    if (!username || !sid_out || sid_out_sz < SID_HEX_LEN + 1) return 0;
    session_cleanup_expired();

    size_t slot = MAX_SESSIONS;
    for (size_t i = 0; i < MAX_SESSIONS; ++i) {
        if (!sessions[i].used) {
            slot = i;
            break;
        }
    }
    if (slot == MAX_SESSIONS) return 0;

    unsigned char random_bytes[32];
    if (RAND_bytes(random_bytes, sizeof(random_bytes)) != 1) return 0;

    char sid[SID_HEX_LEN + 1];
    bytes_to_hex(random_bytes, sizeof(random_bytes), sid, sizeof(sid));

    sessions[slot].used = 1;
    snprintf(sessions[slot].sid, sizeof(sessions[slot].sid), "%s", sid);
    snprintf(sessions[slot].username, sizeof(sessions[slot].username), "%s", username);
    sessions[slot].expires_at = time(NULL) + SESSION_TTL_SECONDS;

    snprintf(sid_out, sid_out_sz, "%s", sid);
    return 1;
}

const char *session_validate(const char *sid) {
    if (!sid || sid[0] == '\0') return NULL;
    session_cleanup_expired();

    for (size_t i = 0; i < MAX_SESSIONS; ++i) {
        if (sessions[i].used && strcmp(sessions[i].sid, sid) == 0) {
            return sessions[i].username;
        }
    }
    return NULL;
}

void session_destroy(const char *sid) {
    if (!sid) return;
    for (size_t i = 0; i < MAX_SESSIONS; ++i) {
        if (sessions[i].used && strcmp(sessions[i].sid, sid) == 0) {
            memset(&sessions[i], 0, sizeof(sessions[i]));
            return;
        }
    }
}
