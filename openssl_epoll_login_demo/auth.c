#include "auth.h"
#include "util.h"
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>

struct user_record {
    const char *username;
    const char *salt;
    const char *sha256_hex; /* SHA256(salt:password) */
};

/* Demo users:
 *   alice / 123456
 *   bob   / password
 * Real systems should use a slow salted password hash such as PBKDF2/bcrypt/Argon2,
 * not plain SHA-256. This demo keeps SHA-256 because the assignment focuses on OpenSSL basics.
 */
static const struct user_record USERS[] = {
    {"alice", "demo_salt_001", "d51a5f6f812b18dd7f1af42f6c82cee497e53bd36115977a9d6034e6509cf566"},
    {"bob",   "demo_salt_002", "6a7dd0ece00a3b4bf07332c6f3877be58e9ef53232481d75a03627f903d91f59"},
};

static int sha256_hex_salt_password(const char *salt, const char *password, char out_hex[65]) {
    unsigned char md[EVP_MAX_MD_SIZE];
    unsigned int md_len = 0;
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) return -1;

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), NULL) != 1 ||
        EVP_DigestUpdate(ctx, salt, strlen(salt)) != 1 ||
        EVP_DigestUpdate(ctx, ":", 1) != 1 ||
        EVP_DigestUpdate(ctx, password, strlen(password)) != 1 ||
        EVP_DigestFinal_ex(ctx, md, &md_len) != 1) {
        EVP_MD_CTX_free(ctx);
        return -1;
    }
    EVP_MD_CTX_free(ctx);
    bytes_to_hex(md, md_len, out_hex);
    return 0;
}

static int constant_time_equal(const char *a, const char *b) {
    size_t la = strlen(a), lb = strlen(b);
    unsigned char diff = (unsigned char)(la ^ lb);
    size_t n = la < lb ? la : lb;
    for (size_t i = 0; i < n; ++i) diff |= (unsigned char)(a[i] ^ b[i]);
    return diff == 0 && la == lb;
}

int auth_verify(const char *username, const char *password) {
    for (size_t i = 0; i < sizeof(USERS) / sizeof(USERS[0]); ++i) {
        if (strcmp(username, USERS[i].username) == 0) {
            char got[65];
            if (sha256_hex_salt_password(USERS[i].salt, password, got) != 0) return 0;
            return constant_time_equal(got, USERS[i].sha256_hex);
        }
    }
    return 0;
}
