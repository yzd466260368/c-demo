#include <stdio.h>

int main(int argc, char const *argv[])
{
    // int a = 10;
    // int *const p = &a; // 指针常量， 不能修改这个指针指向的地址
    // int * p;
    // p = &a;
    // printf("%p", p);

    // 双重限制，既不能修改指针地址，也不能修改指针指向的值
    // 即使指针常量，也是常量指针
    // volatile const int *const p = &a;
    // printf("%lld", p);

    // 数组指针
    int *arr[5];
    int a = 10, b = 20;
    printf("%p\n", arr[1]); // 野指针
    arr[0] = &a;
    arr[1] = &b;
    printf("%p\n", arr[1]);  // 打印指针
    printf("%d\n", *arr[1]); // 打印指针指向的值

    return 0;
}
