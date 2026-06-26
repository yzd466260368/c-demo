#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

    int src_fd, dest_fd;

    if (argc != 3)
    {
        printf("参数错误。");
        return -1;
    }

    // int open(const char *_Filename, int _OpenFlag, ...)

    if ((src_fd = open(argv[1], O_RDONLY)) < 0)
    {
        perror("文件打开失败。");
        return -1;
    }

    if ((dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0)
    {
        perror("文件打开失败。");
        close(src_fd);
        return -1;
    }

    int n;
    char buf[2048];
    while ((n = read(src_fd, buf, sizeof(buf))) > 0)
    {
        write(dest_fd, buf, n);
    }

    if (n < 0)
    {
        printf("文件写入失败");
    }

    close(src_fd);
    close(dest_fd);

    return 0;
}
