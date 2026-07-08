#include "session.h"
#include "util.h"
#include <openssl/rand.h>
#include <string.h>
#include <time.h>

#define MAX_SESSIONS 1024
#define USERNAME_MAX_LEN 63
#define SESSION_TTL_SECONDS 3600

struct session_record {
    int used;
    char sid[SID_HEX_LEN + 1];
    char username[USERNAME_MAX_LEN + 1];
    time_t expires_at;
};

static struct session_record sessions[MAX_SESSIONS];

static void gc_expired(void) {
    time_t now = time(NULL);
    for (size_t i = 0; i < MAX_SESSIONS; ++i) {
        if (sessions[i].used && sessions[i].expires_at <= now) {
            sessions[i].used = 0;
        }
    }
}

int session_create(const char *username, char sid_out[SID_HEX_LEN + 1]) {
    gc_expired();
    size_t slot = MAX_SESSIONS;
    for (size_t i = 0; i < MAX_SESSIONS; ++i) {
        if (!sessions[i].used) { slot = i; break; }
    }
    if (slot == MAX_SESSIONS) return -1;

    unsigned char rnd[SID_BYTES];
    if (RAND_bytes(rnd, sizeof(rnd)) != 1) return -1;
    bytes_to_hex(rnd, sizeof(rnd), sid_out);

    sessions[slot].used = 1;
    strncpy(sessions[slot].sid, sid_out, sizeof(sessions[slot].sid));
    sessions[slot].sid[SID_HEX_LEN] = '\0';
    strncpy(sessions[slot].username, username, USERNAME_MAX_LEN);
    sessions[slot].username[USERNAME_MAX_LEN] = '\0';
    sessions[slot].expires_at = time(NULL) + SESSION_TTL_SECONDS;
    return 0;
}

const char *session_lookup(const char *sid) {
    if (!sid || strlen(sid) != SID_HEX_LEN) return NULL;
    gc_expired();
    for (size_t i = 0; i < MAX_SESSIONS; ++i) {
        if (sessions[i].used && strcmp(sessions[i].sid, sid) == 0) {
            sessions[i].expires_at = time(NULL) + SESSION_TTL_SECONDS;
            return sessions[i].username;
        }
    }
    return NULL;
}

int session_destroy(const char *sid) {
    if (!sid) return 0;
    for (size_t i = 0; i < MAX_SESSIONS; ++i) {
        if (sessions[i].used && strcmp(sessions[i].sid, sid) == 0) {
            sessions[i].used = 0;
            return 1;
        }
    }
    return 0;
}
