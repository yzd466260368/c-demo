#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    /*
    无名管道
    */
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        // 子进程读
        close(fd[1]);

        char buf[128];
        ssize_t n = read(fd[0], buf, sizeof(buf) - 1);

        if (n > 0)
        {
            buf[n] = '\0';
            printf("child read: %s\n", buf);
        }

        close(fd[0]);
        exit(0);
    }
    else
    {
        // 父进程写
        close(fd[0]);

        const char *msg = "hello child";
        write(fd[1], msg, strlen(msg));

        close(fd[1]);
    }

    return 0;
}
