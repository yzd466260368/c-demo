#include <stdio.h>
#include <stdlib.h>

void mall(int **p)
{
    *p = (int *)malloc(sizeof(**p));
}

int main(int argc, char const *argv[])
{
    int *p = NULL;
    mall(&p);
    *p = 10;
    printf("%d\n", *p);
    return 0;
}
