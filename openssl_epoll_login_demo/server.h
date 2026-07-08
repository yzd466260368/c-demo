#ifndef SERVER_H
#define SERVER_H

#include <openssl/ssl.h>

int server_run(int port, SSL_CTX *ctx);

#endif
