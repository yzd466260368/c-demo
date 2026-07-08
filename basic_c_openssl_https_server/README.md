# Basic C + OpenSSL HTTPS Login Server

这是一个“基础 C 语言 + OpenSSL”的 HTTPS 登录示例。

特点：

- C 语言多文件编程
- Makefile 编译
- 服务端使用 `epoll`
- 单进程、单线程
- 不实现客户端代码
- 可使用 `curl` 或 `openssl s_client` 验证
- OpenSSL 只负责：
  - TLS/HTTPS 加密通道
  - SHA256 密码哈希
  - RAND_bytes 生成 Session ID
- HTTP 解析、Cookie、Session、网络通信均由代码自己完成

## 目录结构

```text
.
├── Makefile
├── README.md
├── include
│   ├── config.h
│   ├── http.h
│   ├── session.h
│   ├── ssl_wrap.h
│   ├── user.h
│   └── util.h
└── src
    ├── http.c
    ├── main.c
    ├── session.c
    ├── ssl_wrap.c
    ├── user.c
    └── util.c
```

## 编译运行

先生成自签名证书：

```bash
make cert
```

编译：

```bash
make
```

运行：

```bash
./server 8443
```

浏览器或 curl 访问的是 HTTPS：

```bash
curl -k -i https://127.0.0.1:8443/
```

`-k` 表示跳过自签名证书校验。

## 测试账号

```text
alice / 123456
bob   / password
```

## curl 验证

访问首页：

```bash
curl -k -i https://127.0.0.1:8443/
```

登录并保存 Cookie：

```bash
curl -k -i 'https://127.0.0.1:8443/login?user=alice&password=123456' -c cookie.txt
```

携带 Cookie 查看当前用户：

```bash
curl -k -i https://127.0.0.1:8443/me -b cookie.txt
```

登出：

```bash
curl -k -i https://127.0.0.1:8443/logout -b cookie.txt
```

再次访问 `/me`，应该失败：

```bash
curl -k -i https://127.0.0.1:8443/me -b cookie.txt
```

## openssl s_client 验证

普通 `nc` 不能直接请求 HTTPS，因为它不会进行 TLS 握手。

可以使用：

```bash
openssl s_client -connect 127.0.0.1:8443 -quiet
```

然后手动输入：

```http
GET / HTTP/1.1
Host: localhost
Connection: close

```

## HTTPS 在哪里体现

HTTPS 主要在 `src/main.c` 和 `src/ssl_wrap.c`：

- `SSL_CTX_new(TLS_server_method())`
- `SSL_CTX_use_certificate_file()`
- `SSL_CTX_use_PrivateKey_file()`
- `SSL_new()`
- `SSL_set_fd()`
- `SSL_accept()`
- `SSL_read()`
- `SSL_write()`

也就是：

```text
TCP socket / epoll
        ↓
OpenSSL TLS 加密层
        ↓
自己解析 HTTP
        ↓
登录、Cookie、Session
```

## 说明

本项目为了突出基础 C 实现，Session 使用固定长度数组保存，没有使用数据库、线程池或复杂框架。

密码校验演示使用 SHA256：

- `alice` 的密码 `123456` 保存为 SHA256 十六进制摘要
- `bob` 的密码 `password` 保存为 SHA256 十六进制摘要

正式系统不建议直接使用 SHA256 保存密码，应使用带盐的 PBKDF2、bcrypt、scrypt 或 Argon2。
