#include <stdio.h>

int main(int argc, char const *argv[])
{

    int arr[3][4] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int (*p)[4], i, j;
    p = arr;
    scanf("%d %d", &i, &j);
    printf("%d\n", *(*(p + i) + j));

    return 0;
}

// int main(int argc, char const *argv[])
// {

//     /* 编写一个程序，使用goto 跳出二层嵌套循环 */

//     for (int i = 0; i < 10; i++)
//     {
//         for (int j = i + 1; j < 10; j++)
//         {
//             printf("\n");
//             if (j > 1)
//             {
//                 break;
//             }
//             printf("j = %d\n", j);
//         }
//         printf("i = %d\n", i);
//     }

//     return 0;
// }

// int countDigits(long long num)
// {

//     if ((num / 10) == 0)
//     {
//         return 1;
//     }
//     int count = 0;
//     while (num != 0)
//     {
//         num /= 10;
//         count++;
//     }
//     return count;
// }

// int main(int argc, char const *argv[])
// {
//     /* 请编写一个程序，定义一个函数countDigits，计算一个整数中数字的个数 */
//     long long a;
//     printf("请输入一个整数\n");
//     scanf("%lld", &a);
//     int result = countDigits(a);
//     printf("%d这个整数中一共有%d个数字\n", a, result);
//     return 0;
// }

// #include <string.h>

// typedef struct Person
// {
//     char name[20];
//     int age;
// } Person1;

// int main(int argc, char const *argv[])
// {

//     /*请编写一个程序，定义一个结构体 Person ，包含name（字符串）和age（整数）。通过scanf和printf输入输出该结构体的成员*/
//     char name[20];
//     int age;
//     printf("定义一个结构体Person，请输入name和age\n");
//     scanf("%19s %d", name, &age);
//     struct Person p;
//     strncpy(p.name, name, sizeof(p.name) - 1);
//     p.name[sizeof(p.name) - 1] = '\0';
//     p.age = age;
//     printf("结构体定义成功，name = %s，age = %d", p.name, p.age);

//     return 0;
// }