#include <stdio.h>

int main(int argc, char const *argv[])
{

    char a, b;
    printf("请输入两个字符。\n");
    scanf("%c %c", &a, &b);
    printf("%c %c\n", a, b);
    if ((a >= 'a' && a <= 'z') || (b >= 'a' && b <= 'z'))
    {
        printf("请输入大写字母。\n");
        return -1;
    }
    a += 32;
    b += 32;
    printf("%c %c\n", a, b);

    return 0;
}