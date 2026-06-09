#include <stdio.h>

int main(int argc, char const *argv[])
{

    /*请编写一个程序，定义一个结构体Person，包含name（字符串）和age（整数）。通过scanf和printf输入输出该结构体的成员*/
    return 0;
}

// #define ARR_NUM 5

// void *reverseArray(int *arr, int num)
// {
//     int max = num - 1;
//     int min = 0;
//     while (max > min)
//     {
//         int t = *(arr + max);
//         *(arr + max) = *(arr + min);
//         *(arr + min) = t;
//         max--;
//         min++;
//     }
// }

// int main(int argc, char const *argv[])
// {
//     /*请编写一个程序，在reverseArray()函数中使用指针实现数组的反转，reverseArray函数有两个输入参数，一个是数组的指针，一个是数组中元素的个数，数组初始有5个元素*/
//     int arr[ARR_NUM] = {1, 2, 3, 4, 5};
//     reverseArray(arr, ARR_NUM);

//     for (int i = 0; i < ARR_NUM; i++)
//     {
//         printf("%d\n", arr[i]);
//     }

//     return 0;
// }

// int main(int argc, char const *argv[])
// {
//     /*请编写一个程序，使用scanf为一个有10个元素的数组初始化0-10的数字，统计其中每个元素出现的次数*/
//     int arr[10];
//     printf("请输入10个0-10的数字\n");
//     int count = 0;
//     while (count < 10)
//     {
//         int a;
//         scanf("%d", &a);
//         if (a >= 0 && a <= 10)
//         {
//             arr[count] = a;
//             count++;
//         }
//         else
//         {
//             printf("请输入0-10的数字\n");
//         }
//     }
//     printf("数组初始化完成\n");

//     // 遍历数组，统计每个元素出现的次数
//     int result[11] = {0};
//     for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
//     {
//         result[arr[i]]++;
//     }

//     for (int i = 0; i <= 10; i++)
//     {
//         printf("%d 出现了 %d 次\n", i, result[i]);
//     }

//     return 0;
// }

// #define ADD '+'
// #define SUB '-'
// #define MULTI '*'
// #define DIVI '/'

// int main(int argc, char const *argv[])
// {
//     // 用户输入两个数和一个操作符（+-*/），根据操作符进行相应的运算，并输出结果
//     // 使用 switch 语句处理不同的操作符情况，并检查除法操作是否有零作为除数。

//     int a, b;
//     char ch;
//     printf("请输入两个数和一个运算符\n");
//     scanf("%d %d %c", &a, &b, &ch);

//     int result;
//     switch (ch)
//     {
//     case ADD:
//         result = a + b;
//         break;
//     case SUB:
//         result = a - b;
//         break;
//     case MULTI:
//         result = a * b;
//         break;
//     case DIVI:
//         if (b != 0)
//         {
//             result = a / b;
//         }
//         else
//         {
//             printf("除数不能为0\n");
//             return -1;
//         }
//         break;

//     default:
//         printf("操作符不支持。\n");
//         return -1;
//         break;
//     }
//     printf("运算结果是：%d %c %d = %d", a, ch, b, result);

//     return 0;
// }

// int findL(char *str, char ch)
// {
//     for (int i = 0; str[i] != '\0'; i++)
//     {

//         if (str[i] == ch)
//         {
//             return i + 1;
//         }
//     }
//     return -1;
// }

// int main(int argc, char const *argv[])
// {
//     /*
//     计算字符t在str数组中第一次出现的位置
//     如str为“abcc”，t为’c’，则返回3。如果没有找到该字符，则返回-1。
//     */
//     char ch = 'c';
//     char str[] = "abcc";
//     int result = findL(str, ch);
//     if (result == -1)
//     {
//         printf("%c未找到\n", ch);
//     }
//     else
//     {
//         printf("%c字符出现在位置字符串%s的第%d位\n", ch, str, result);
//     }

//     return 0;
// }
