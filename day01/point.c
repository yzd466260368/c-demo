#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main(void)
{

    int a = 1;
    int *p = &a;

    /* 打印指针地址（正确做法） */
    printf("address: %p\n", (void *)p);

    /* 解引用：打印指针指向的值 */
    printf("value: %d\n", *p);

    /* 若需把地址作为整数处理，使用 uintptr_t */
    uintptr_t addr = (uintptr_t)p;
    printf("address as integer: %" PRIuPTR "\n", addr);

    return 0;
}
