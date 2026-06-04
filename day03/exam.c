#include <stdio.h>

int main(int argc, char const *argv[])
{

    /*
    要求用户输入一行文本，并输出该文本
    */
    char a[100] = {0};
    // printf("%d", (sizeof(a) / sizeof(a[0]) - 1));
    printf("请输入一行文本");
    if (fgets(a, (sizeof(a) / sizeof(a[0])) - 1, stdin) != NULL)
    {
        puts(a);
    }
    return 0;
}

/*
使用 scanf 读取整数和浮点数，使用 printf 输出
 */
// int a;
// float b;
// printf("请输入一个整数和一个浮点数\n");
// scanf("%d %f", &a, &b);
// printf("整数为：%d，浮点数为：%-6.2f\n", a, b);

/*
使用 fgets 获取用户输入的字符串，并通过 fputs 输出。
*/
// char a[100];
// printf("请输入一个字符串");
// if (fgets(a, 99, stdin) != NULL)
// {
//     fputs(a, stdout);
// }

/*
要求用户输入一个整数，并输出该整数
*/
// int a;
// printf("请输入一个整数");
// scanf("%d", &a);
// printf("%d", a);

/*
通过 fgets 读取两个字符串，并使用 printf 输出。
*/

// char a[100];
// char b[100];
// printf("请输入第一个字符串");
// if (fgets(a, 99, stdin) != NULL)
// {
//     printf("%s", a);
// }
// printf("请输入第二个字符串");
// if (fgets(b, 99, stdin) != NULL)
// {
//     printf("%s", b);
// }
// printf("请输入第一个字符串");
// if (fgets(a, 99, stdin) != NULL)
// {
//     printf("请输入第二个字符串");
//     if (fgets(b, 99, stdin) != NULL)
//     {
//         printf("%s", a);
//         printf("%s", b);
//     }
// }

/*
使用 scanf 获取两个整数，并通过 printf 输出它们的和。
*/
// int a, b;
// printf("请输入两个整数\n");
// scanf("%d %d", &a, &b);
// printf("sum = %d", (a + b));

/*
用户输入一个字符，并输出该字符
*/

// char a = getchar();
// putchar(a);