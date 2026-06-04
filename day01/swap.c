#include <stdio.h>
void swap(int *a, int *b);

int main(int argc, char const *argv[])
{
    int x = 10;
    int y = 20;
    swap(&x, &y);
    printf("x = %d, y = %d\n", x, y);
    return 0;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
