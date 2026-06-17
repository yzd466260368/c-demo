#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char const *argv[])
{

    // FILE *file = fopen("/home/yuzd/a.txt", "r");

    // if (file == NULL)
    // {
    //     perror("");
    //     return -1;
    // }
    // printf("文件打开成功\n");

    // char buf[BUFSIZ];

    // while (fgets(buf, sizeof(buf), file) != NULL)
    // {
    //     printf("%s", buf);
    // }

    // int c = fclose(file);
    // if (c == EOF)
    // {
    //     perror("");
    //     return -1;
    // }

    /*
    fgets
    fputs
    fgetc
    fputc

    setbuf
    setvbuf
    */
    printf("行缓冲区的大小：%td\n",
           stdout->_IO_buf_end - stdout->_IO_buf_base);
    printf("hello\n");
    // printf("stdout buffer size = %zu\n", __fbufsize(stdout));
    printf("行缓冲区的大小：%td\n",
           stdout->_IO_buf_end - stdout->_IO_buf_base);

    // setvbuf(FILE * fp, char *buf, _IOFBF, 4096);
    return 0;
}
