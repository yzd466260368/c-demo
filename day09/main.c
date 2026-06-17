#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char a;   // 1
    double b; // 8
    int c;    // 4
} aa;

char *my_strcpy(char *dest, const char *src)
{
    char *ret = dest;

    while ((*dest++ = *src++) != '\0')
        ;

    return ret;
}

int main()
{
    // int a = 10;
    // int *p1 = &a;

    // int b = 20;
    // int *p2 = &b;

    // int **pp = &p1; // pp 指向一级指针 p1

    // printf("修改前：\n");
    // printf("*p1 = %d\n", *p1); // 10，p1 指向a

    // // 核心：*pp 就是 p1，直接给 p1 赋值新地址 &b
    // *pp = &b;

    // printf("修改后：\n");
    // printf("*p1 = %d\n", *p1); // 20，p1 现在指向b

    // printf("内存大小：%d\n", sizeof(aa));

    char *a = "hello";
    char *b;
    my_strcpy(b, a);

    printf("%s\n", b);

    // int *c = (int *)malloc(5 * sizeof(int));

    return 0;
}

void func()
{
    int a = 10;
    printf("%d\n", a);

    /*
    int a = 10;
    a = 20;  // 合法，修改变量a，不是修改常量10
    10 = 20; // 编译报错！常量不能被赋值修改
    */
}