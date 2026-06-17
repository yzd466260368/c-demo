#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    // int *arr[10];
    // int *a = malloc(10 * sizeof(int));
    // int arr[10] = {1, 4, 3};
    // int a = *(arr + 1);
    // printf("%d\n", a);
    // int arr[5] = {1, 2, 3, 4, 5};
    // int *p = arr;
    // printf("%d\n", *(p + 1));

    // void *p;
    // int b = 20;
    // int *i = &b;
    // p = i;
    // printf("%d\n", *p);

    // int a = 10;
    // char *p = &a;
    // printf("%c\n", *(int *)p);

    const int a = 10;
    int *p = &a;
    *p = 30;
    printf("%d\n", a);

    return 0;
}
