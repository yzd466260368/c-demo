#include <stdio.h>
#include <stdlib.h>

int fibonacci(int num)
{
    printf("%d\n", num);
    if (num <= 1)
    {
        return num;
    }
    return fibonacci(num - 1) + fibonacci(num - 2);
}

int main()
{
    // char a[5] = {'A', 'B', 'C', 'D'};
    // char (*p3)[3] = &a;
    // char (*p4)[3] = a;
    // char (*p3)[3] = (char (*)[3]) & a;
    // char (*p4)[3] = (char (*)[3])a;

    // int c[5] = {1, 2, 3, 4, 5};
    // int (*p)[5] = &c;

    // *(int *)((int)p + 1);
    // *(int *)((char *)p + 1);

    // int *p = NULL;
    // printf("%d\n", sizeof(p));  // 32 : 4 64 : 8
    // printf("%d\n", sizeof(*p)); // 4
    // int a[100];
    // printf("%d\n", sizeof(a));      // 100 * 4
    // printf("%d\n", sizeof(a[100])); // 4， 这里数组角标不是越界了吗， sizeof(a[100])不算越界，a[100]是越界
    // printf("%d\n", sizeof(&a));     // 32 : 4 64 : 8
    // printf("%d\n", sizeof(&a[0]));  // 4 这里是指针类型，所以是8

    // int *p = NULL;
    // int p = 10;
    // printf("%ld\n", sizeof(int) * p); // 4 * 10 = 40

    // int res = fibonacci(5);

    // printf("res = %d\n", res);

    // int **p;
    // p = malloc(sizeof(*p));
    // *p = malloc(sizeof(**p));

    // int a[] = {1, 2, 3};

    // int *p = a;

    // printf("%d\n", *p++);

    // const int *q;  // *q指向的数据只读不可修改
    // int *const qq; // qq的数据只读不可修改

    // int x;
    // int i = 3;

    // x = (++i, i++, i + 10);

    // printf("%d\n", x);

    // int a[5] = {1, 2, 3, 4, 5};
    // printf("%p\n", a);
    // printf("%p\n", a + 1);
    // printf("%p\n", a + 2);
    // printf("%p\n", a + 3);
    // printf("%p\n", a + 4);
    // printf("%p\n", &a + 1);

    // int *p1 = a + 1;
    // int *p2 = a + 4;
    // printf("%d\n", p2 - p1);

    // int a = 100;

    // int *p = NULL;

    // // p = &a;

    // printf("%p\n", p);

    char a[5] = {'A', 'B', 'C', 'D', 0};
    char (*p3)[10] = (char (*)[10]) & a;
    printf("%p\n", &a);
    printf("%p\n", &a + 1);
    printf("%p\n", p3);
    printf("%p\n", p3 + 1);
    char (*p4)[3] = (char (*)[3])a;

    return 0;
}
