#include <stdio.h>

int main(int argc, char const *argv[])
{

    char a;
    a = getchar();
    printf("%c\n", a);
    a = getchar();
    printf("%c\n", a);

    // char a = '1';
    // ++a;
    // printf("%c", a);

    /*
    scanf()
    getchar()
    fgets()
    printf()
    putchar()
    puts()


    gets(): C99存在， 后面移除

    */
    // char a[100];
    // printf("请输入一串字符");
    // if (fgets(a, 10, stdin) != NULL)
    // {
    //     printf("你输入的字符串：%s\n", a);
    // }
    // else
    // {
    //     printf("输入不正确。");
    // }
    // unsigned auto char a = 'A';
    // putchar(a);
    // char a[100] = {0};
    // printf("请输入一个字符串：\n");
    // scanf("%s", a);
    // printf("%s", a);

    return 0;
}