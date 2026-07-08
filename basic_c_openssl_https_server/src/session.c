#include "session.h"
#include "config.h"
#include "util.h"

#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct session_item {
    int active;
    char sid[65];
    char username[32];
    time_t expire_at;
};

static struct session_item g_sessions[MAX_SESSIONS];

void session_init(void) {
    memset(g_sessions, 0, sizeof(g_sessions));
}

void session_cleanup_expired(void) {
    time_t now = time(NULL);
    size_t i;

    for (i = 0; i < MAX_SESSIONS; i++) {
        if (g_sessions[i].active && g_sessions[i].expire_at <= now) {
            memset(&g_sessions[i], 0, sizeof(g_sessions[i]));
        }
    }
}

int session_create(const char *username, char *sid_out, size_t sid_out_size) {
    unsigned char random_bytes[32];
    char sid[65];
    time_t now = time(NULL);
    size_t i;

    if (username == NULL || sid_out == NULL || sid_out_size < sizeof(sid)) {
        return 0;
    }

    session_cleanup_expired();

    if (RAND_bytes(random_bytes, sizeof(random_bytes)) != 1) {
        return 0;
    }
    bytes_to_hex(random_bytes, sizeof(random_bytes), sid);

    for (i = 0; i < MAX_SESSIONS; i++) {
        if (!g_sessions[i].active) {
            g_sessions[i].active = 1;
            snprintf(g_sessions[i].sid, sizeof(g_sessions[i].sid), "%s", sid);
            snprintf(g_sessions[i].username, sizeof(g_sessions[i].username), "%s", username);
            g_sessions[i].expire_at = now + SESSION_SECONDS;
            strncpy(sid_out, sid, sid_out_size - 1);
            sid_out[sid_out_size - 1] = '\0';
            return 1;
        }
    }

    return 0;
}

const char *session_get_user(const char *sid) {
    time_t now = time(NULL);
    size_t i;

    if (sid == NULL || sid[0] == '\0') {
        return NULL;
    }

    for (i = 0; i < MAX_SESSIONS; i++) {
        if (g_sessions[i].active && strcmp(g_sessions[i].sid, sid) == 0) {
            if (g_sessions[i].expire_at <= now) {
                memset(&g_sessions[i], 0, sizeof(g_sessions[i]));
                return NULL;
            }
            g_sessions[i].expire_at = now + SESSION_SECONDS;
            return g_sessions[i].username;
        }
    }

    return NULL;
}

void session_destroy(const char *sid) {
    size_t i;

    if (sid == NULL || sid[0] == '\0') {
        return;
    }

    for (i = 0; i < MAX_SESSIONS; i++) {
        if (g_sessions[i].active && strcmp(g_sessions[i].sid, sid) == 0) {
            memset(&g_sessions[i], 0, sizeof(g_sessions[i]));
            return;
        }
    }
}
