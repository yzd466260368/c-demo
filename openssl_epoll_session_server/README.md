# C epoll + OpenSSL Session Demo

This is a single-process, single-thread HTTPS server written in C.

Features:

- OpenSSL TLS server
- epoll event loop
- simple HTTP parser
- login/logout/session cookie flow
- Makefile multi-file build

## Build

```bash
make cert
make
./server 8443
```

## Test with curl

```bash
curl -k -i https://127.0.0.1:8443/

curl -k -i 'https://127.0.0.1:8443/login?user=alice&password=123456' -c cookie.txt
curl -k -i https://127.0.0.1:8443/me -b cookie.txt
curl -k -i https://127.0.0.1:8443/logout -b cookie.txt
curl -k -i https://127.0.0.1:8443/me -b cookie.txt
```

Demo users:

- alice / 123456
- bob / password

## Test manually

Plain `nc` cannot speak TLS. Use `openssl s_client` instead:

```bash
openssl s_client -connect 127.0.0.1:8443 -quiet
GET / HTTP/1.1
Host: localhost
Connection: close

```
