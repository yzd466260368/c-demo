#include <stdio.h>

typedef struct
{
    char **name[10];
    int count;
    int capcity;
} Student;

int add(int a, int b)
{
    return a + b;
}

int multiply(int a, int b)
{
    return a * b;
}

int compute(int (*operation)(int, int), int a, int b)
{
    return operation(a, b);
}

int main(int argc, char const *argv[])
{
    // int a = 1, b = 2, c = 3;
    // int *strArr[10] = {&a, &b};
    // // char **strArr1[10];
    // strArr[0] = &c;
    // printf("%d\n", *strArr[0]);

    int arr[5] = {1, 2, 3, 4, 5};

    int (*p)[5] = &arr; // 数组指针，指向这整个数组
    /*
    int *p; 普通指针，指向一个int变量
    int *p[5]; 指针数组，每个元素是一个指针
    int (*p)[5]; 数组指针，指向的是这一整个数组
    */
    printf("%d\n", (*p)[0]);
    printf("%d\n", arr[0]);

    int arr2[3][3] = {
        {1, 2, 3},
        {1, 2, 3},
        {1, 2, 3}};

    printf("%d\n", sizeof(arr2));
    /*
    数值上是一样的，但是意义上不一样
    */
    printf("%p\n", arr);
    printf("%p\n", &arr[0]);
    printf("%p\n", &arr);

    // 函数指针
    int c = 10, d = 20;
    int (*fun)(int, int);
    fun = add;
    printf("两数之和：%d\n", fun(c, d));
    fun = multiply;
    printf("两数之积：%d\n", fun(c, d));

    printf("两数之和：%d\n", compute(add, c, d));
    printf("两数之积：%d\n", compute(multiply, c, d));

    // 函数指针数组
    int (*fun1[5])(int, int);

    // 二级指针
    int e = 40;

    int *q = &e;
    int **w = &q;

    printf("%d\n", **w);
    printf("%p\n", q);
    printf("%p\n", w);
    printf("%p\n", *w);
    printf("%d\n", **w);

    return 0;
}

int *max(int *a, int *b)
{
    if (*a > *b)
    {
        return a;
    }
    else
    {
        return b;
    }
}
