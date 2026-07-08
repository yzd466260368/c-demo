#include "user.h"
#include "util.h"

#include <openssl/sha.h>
#include <string.h>

struct user_record {
    const char *username;
    const char *password_sha256_hex;
};

/* Demo users:
 *   alice / 123456
 *   bob   / password
 * In real systems, use salted password hashing/KDF such as PBKDF2, bcrypt, scrypt or Argon2.
 */
static const struct user_record USERS[] = {
    {"alice", "8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92"},
    {"bob",   "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8"},
};

int user_verify_password(const char *username, const char *password) {
    if (!username || !password) return 0;

    unsigned char digest[SHA256_DIGEST_LENGTH];
    char hex[SHA256_DIGEST_LENGTH * 2 + 1];

    SHA256((const unsigned char *)password, strlen(password), digest);
    bytes_to_hex(digest, sizeof(digest), hex, sizeof(hex));

    for (size_t i = 0; i < sizeof(USERS) / sizeof(USERS[0]); ++i) {
        if (strcmp(username, USERS[i].username) == 0 &&
            strcmp(hex, USERS[i].password_sha256_hex) == 0) {
            return 1;
        }
    }
    return 0;
}
