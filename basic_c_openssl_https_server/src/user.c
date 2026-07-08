#include "user.h"
#include "util.h"

#include <openssl/sha.h>
#include <string.h>

struct user_record {
    const char *name;
    const char *password_sha256_hex;
};

/*
 * Demo users:
 *   alice / 123456
 *   bob   / password
 *
 * Real systems should use salted password hashing such as PBKDF2, bcrypt,
 * scrypt, or Argon2. SHA256 is used here only to show OpenSSL hash usage.
 */
static const struct user_record g_users[] = {
    {"alice", "8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92"},
    {"bob",   "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8"}
};

int user_check_password(const char *username, const char *password) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    char hex[SHA256_DIGEST_LENGTH * 2 + 1];
    size_t i;

    if (username == NULL || password == NULL) {
        return 0;
    }

    SHA256((const unsigned char *)password, strlen(password), digest);
    bytes_to_hex(digest, SHA256_DIGEST_LENGTH, hex);

    for (i = 0; i < sizeof(g_users) / sizeof(g_users[0]); i++) {
        if (strcmp(username, g_users[i].name) == 0 &&
            strcmp(hex, g_users[i].password_sha256_hex) == 0) {
            return 1;
        }
    }

    return 0;
}
