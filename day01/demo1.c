#include <stdio.h>
#include <stdlib.h>

int global = 100;

void func(int n)
{
    int a = 10;
    int *p = malloc(sizeof(int));

    *p = 20;

    printf("%d %d %d\n", n, a, *p);

    free(p);
}

int main()
{
    func(5);
    return 0;
}