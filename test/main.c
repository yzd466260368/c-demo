#include <stdio.h>
#include "a.h"

extern int c;
void test2();

int main(int argc, char const *argv[])
{
    test2();
    printf("%d\n", c);
    return 0;
    // printf("%d\n", num);
    // hello();

    // num = 20;
    // hello();

    // return 0;
}
