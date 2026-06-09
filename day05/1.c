#include <stdio.h>

// void show();

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(int argc, char const *argv[])
{
    printf("hello world\n");
    /*
    D:/DevCodes/c-demo/day05/1.c:8:(.text+0x24): undefined reference to `show'
    collect2.exe: error: ld returned 1 exit status
    链接错误
    */
    // show();

    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    printf("%ld\n", sizeof(long));
    printf("%ld\n", sizeof(long long));

    /*
    大端小端判断
    */
    int a = 0x12345678;
    char *p = (char *)&a;
    if (*p == 0x78)
    {
        printf("%d\n", *p);
        printf("该系统是小端序\n");
    }
    else
    {
        printf("该系统是大端序\n");
    }

    // char *q = "123456"; // 定义字符串常量，字符串分配在常量区
    // const char *q = "123456"; // 同上
    // char q[] = "123456"; // 字符串分配在栈区
    // *q = 'A';
    // printf("%s\n", q);

    /*
    指针的运算
    */
    int c = 30;
    int *r = &c;
    printf("%016p\n", r);
    r++; // 指向下一个地址
    printf("%016p\n", r);
    if (r == NULL)
    {
        /* code */
    }

    int t = 11;
    if (1 < t < 10) // 恒为真
    {
        printf("我执行了\n");
    }

    printf("%d\n", !1);

    return 0;
}
