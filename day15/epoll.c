#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define MAX_EVENTS 1024
#define BUF_SIZE 1024

int main()
{
    int listenfd, connfd, epfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    struct epoll_event ev, events[MAX_EVENTS];
    char buf[BUF_SIZE];

    // 1. 创建 socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("socket");
        return -1;
    }

    // 2. 绑定 IP 和端口
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(listenfd);
        return -1;
    }

    // 3. 监听
    if (listen(listenfd, 128) < 0)
    {
        perror("listen");
        close(listenfd);
        return -1;
    }

    // 4. 创建 epoll
    epfd = epoll_create(1);
    if (epfd < 0)
    {
        perror("epoll_create");
        close(listenfd);
        return -1;
    }

    // 5. 把监听套接字加入 epoll
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev) < 0)
    {
        perror("epoll_ctl");
        close(listenfd);
        close(epfd);
        return -1;
    }

    printf("epoll tcp server start, port = %d\n", PORT);

    // 6. 等待事件
    while (1)
    {
        int nready = epoll_wait(epfd, events, MAX_EVENTS, -1);

        if (nready < 0)
        {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nready; i++)
        {
            int fd = events[i].data.fd;

            // 7. 如果是监听套接字，说明有新客户端连接
            if (fd == listenfd)
            {
                client_len = sizeof(client_addr);

                connfd = accept(listenfd,
                                (struct sockaddr *)&client_addr,
                                &client_len);

                if (connfd < 0)
                {
                    perror("accept");
                    continue;
                }

                printf("client connected: %s:%d, fd = %d\n",
                       inet_ntoa(client_addr.sin_addr),
                       ntohs(client_addr.sin_port),
                       connfd);

                // 把客户端套接字加入 epoll
                ev.events = EPOLLIN;
                ev.data.fd = connfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            }
            // 8. 如果是客户端套接字，说明有数据可读
            else
            {
                memset(buf, 0, sizeof(buf));

                int len = recv(fd, buf, sizeof(buf), 0);

                if (len > 0)
                {
                    printf("recv: %s\n", buf);

                    // 回显数据
                    send(fd, buf, len, 0);
                }
                else if (len == 0)
                {
                    printf("client closed, fd = %d\n", fd);

                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                }
                else
                {
                    perror("recv");

                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                }
            }
        }
    }

    close(listenfd);
    close(epfd);

    return 0;
}