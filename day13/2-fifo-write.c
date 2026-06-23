#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    /* 有名管道FIFO - 写 */

    int fd = open("myfifo", O_WRONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    const char *msg = "hello fifo";
    write(fd, msg, strlen(msg));

    close(fd);
    return 0;
}
