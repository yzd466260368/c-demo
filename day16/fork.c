#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        printf("fork failed\n");
    }
    else if (pid == 0)
    {
        printf("我是子进程\n");
        printf("子进程pid = %d，父进程pid = %d\n", getpid(), getppid());
    }
    else
    {
        printf("我是父进程\n");
        printf("父进程pid = %d，子进程pid = %d\n", getpid(), pid);
    }

    return 0;
}
