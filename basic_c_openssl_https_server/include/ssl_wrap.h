#ifndef SSL_WRAP_H
#define SSL_WRAP_H

#include <openssl/ssl.h>

void ssl_global_init(void);
void ssl_global_cleanup(void);
SSL_CTX *ssl_create_server_ctx(const char *cert_file, const char *key_file);
void ssl_print_error(const char *msg);

#endif
