#ifndef TLS_H
#define TLS_H

#include <openssl/ssl.h>

SSL_CTX *tls_create_server_ctx(const char *cert_file, const char *key_file);

#endif
