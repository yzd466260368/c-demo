#include <stdio.h>
#include "a.h"

int num = 10; // 定义全局变量，只能定义一次

void hello(void)
{ // 定义函数
    printf("hello, num = %d\n", num);
}