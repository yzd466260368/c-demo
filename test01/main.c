#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

    int src, dest;
    char buf[2048];
    int n = 0;

    if (argc != 3)
    {
        printf("参数错误。\n");
        return -1;
    }

    if ((src = open(argv[1], O_RDONLY)) < 0)
    {
        perror("open src");
        return -1;
    }

    if ((dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0)
    {
        perror("open dest");
        close(src);
        return -1;
    }

    while ((n = read(src, buf, sizeof(buf))) > 0)
    {
        if (write(dest, buf, n) < n)
        {
            perror("write");
            close(src);
            close(dest);
            return -1;
        }
    }

    if (n < 0)
    {
        perror("read");
    }

    close(src);
    close(dest);

    return 0;
}

// int main()
// {
//     int a[5] = {1, 2, 3, 4, 5};
//     int *p = a;
//     int b = *(int *)((char *)p + 1);

//     printf("%d\n", b);
//     return 0;
// }