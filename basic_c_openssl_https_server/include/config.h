#ifndef CONFIG_H
#define CONFIG_H

#define DEFAULT_PORT 8443
#define LISTEN_BACKLOG 128
#define MAX_EVENTS 128
#define MAX_SESSIONS 128
#define SESSION_SECONDS 1800

#define CERT_FILE "cert/server.crt"
#define KEY_FILE  "cert/server.key"

#define IN_BUF_SIZE 8192
#define OUT_BUF_SIZE 8192

#endif
