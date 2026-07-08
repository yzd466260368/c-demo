#include "ssl_wrap.h"

#include <openssl/err.h>
#include <stdio.h>

void ssl_global_init(void) {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void ssl_global_cleanup(void) {
    EVP_cleanup();
}

void ssl_print_error(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    ERR_print_errors_fp(stderr);
}

SSL_CTX *ssl_create_server_ctx(const char *cert_file, const char *key_file) {
    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());

    if (ctx == NULL) {
        ssl_print_error("SSL_CTX_new failed");
        return NULL;
    }

    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);

    if (SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) != 1) {
        ssl_print_error("Failed to load certificate file");
        SSL_CTX_free(ctx);
        return NULL;
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) != 1) {
        ssl_print_error("Failed to load private key file");
        SSL_CTX_free(ctx);
        return NULL;
    }

    if (SSL_CTX_check_private_key(ctx) != 1) {
        ssl_print_error("Certificate and private key do not match");
        SSL_CTX_free(ctx);
        return NULL;
    }

    return ctx;
}
