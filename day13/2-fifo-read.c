#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    /* 有名管道FIFO - 读 */

    int fd = open("myfifo", O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    char buf[128];

    ssize_t n = read(fd, buf, sizeof(buf) - 1);

    if (n > 0)
    {
        buf[n] = '\0';
        printf("read from fifo: %s\n", buf);
    }

    close(fd);
    return 0;
}
