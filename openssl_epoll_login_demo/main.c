#include "server.h"
#include "tls.h"
#include <openssl/ssl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <port> <cert.pem> <key.pem>\n", argv[0]);
        fprintf(stderr, "Example: %s 8443 server.crt server.key\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "invalid port\n");
        return 1;
    }

    SSL_CTX *ctx = tls_create_server_ctx(argv[2], argv[3]);
    if (!ctx) {
        fprintf(stderr, "failed to create TLS context\n");
        return 1;
    }

    int rc = server_run(port, ctx);
    SSL_CTX_free(ctx);
    return rc;
}
