#include <stdio.h>
int main(int argc, char const *argv[])
{
    int arr[5] = {1, 2, 3, 4, 5};
    int *p = arr;

    printf("0x%x\n", *(int *)(p + 1));
    return 0;
}
