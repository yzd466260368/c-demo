#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    pid_t pid;

    /* 第一次 fork */
    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS); // 父进程退出

    /* 创建新会话 */
    setsid();

    /* 第二次 fork */
    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* 修改工作目录 */
    chdir("/");

    /* 清除文件权限掩码 */
    umask(0);

    /* 关闭标准输入输出错误 */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /* 后台运行 */
    while (1)
    {
        sleep(5);
        // 执行后台任务
    }

    return 0;
}