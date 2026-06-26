#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int src_fd, dst_fd;
    char buf[1024];
    ssize_t n;

    src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0)
    {
        perror("open src_fd error\n");
        return -1;
    }

    dst_fd = open(argv[2], _O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (dst_fd < 0)
    {
        perror("open dst_fd error\n");
        close(src_fd);
        return -1;
    }

    while ((n = read(src_fd, buf, sizeof(buf))) > 0)
    {
        if (write(dst_fd, buf, n) != n)
        {
            perror("write");
            close(src_fd);
            close(dst_fd);
            return -1;
        }
    }
    if (n < 0)
    {
        perror("read");
    }

    close(src_fd);
    close(dst_fd);

    return 0;
}
