#include <stdio.h>
#include <stdint.h>

void printf1(const char *ch);

int main(int argc, char const *argv[])
{
    uint8_t a = 12;
    printf("%d\n", a);
    printf("hello world.\n");
    printf1("hello");
    return 0;
}

void printf1(const char *ch)
{
    printf("%s", ch);
}
