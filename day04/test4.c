#include <stdio.h>
// #include <stdlib.h>
#include <stdbool.h>

int findFirstShow(char *str, char t)
{
    int count = 1;
    bool flag = false;
    while (*str != '\0')
    {
        if (*str == t)
        {
            flag = true;
            break;
        }
        str++;
        count++;
    }
    if (flag)
    {
        return count;
    }
    else
    {
        return -1;
    }
}

int main(int argc, char const *argv[])
{
    /*编写一个函数，用来计算字符t在str数组中第一次出现的位置
    如str为“abcc”，t为’c’，则返回3。如果没有找到该字符，则返回-1。*/
    char str[] = "abcc";
    char t = 'c';
    int result = findFirstShow(str, t);
    if (result != -1)
    {
        printf("该字符在字符串中的位置是：%d", result);
    }
    else
    {
        printf("字符未找到。");
    }

    return 0;
}

// bool areStringEqual(char *str1, char *str2)
// {
//     while (*str1 != '\0' || *str2 != '\0')
//     {
//         if (*str1 != *str2)
//             return false;
//         str1++;
//         str2++;
//     }
//     return true;
// }

// int main(int argc, char const *argv[])
// {
//     /*在areStringEqual()函数中使用两个指针作为输入参数判断两个字符串是否相等*/
//     char str1[] = "hello";
//     char str2[] = "hello123";
//     bool flag = areStringEqual(str1, str2);
//     if (flag)
//     {
//         printf("两个字符串相等。");
//     }
//     else
//     {
//         printf("两个字符串不相等。");
//     }

//     return 0;
// }

// void concatenateStrings(char *str1, char *str2, char *retStr)
// {

//     while (*str1 != '\0')
//     {
//         *retStr = *str1;
//         str1++;
//         retStr++;
//     }

//     while (*str2 != '\0')
//     {
//         *retStr = *str2;
//         str2++;
//         retStr++;
//     }

//     *retStr = '\0';
// }

// int main(int argc, char const *argv[])
// {
//     /* 在concatenateStrings()函数中使用指针将两个字符串Hello和World！连接起来，
//     concatenateStrings函数的输入参数有三个，一个是第一个字符串的指针，第二个是第二个字符串的指针，第三个是用于存储连接结果的字符数组 */
//     char str1[] = "Hello";
//     int strLen1 = sizeof(str1) / sizeof(str1[0]);
//     char str2[] = "World！";
//     int strLen2 = sizeof(str2) / sizeof(str2[0]);
//     char *concatStr = malloc((strLen1 + strLen2) * sizeof(char));
//     concatenateStrings(str1, str2, concatStr);
//     printf("%s\n", concatStr);
//     free(concatStr);
//     concatStr = NULL;
//     return 0;
// }

// #define LEN 5

// int findMax(int *arr, int len)
// {
//     int max = 0;
//     for (int i = 0; i < len; i++)
//     {
//         if (arr[i] > max)
//         {
//             max = arr[i];
//         }
//     }
//     return max;
// }

// int main(int argc, char const *argv[])
// {
//     /* 在findMax()函数中输入数组指针和元素数量，使用指针在数组中查找最大值，返回最大值给主函数并在主函数打印出来 */
//     int arr[LEN] = {1, 2, 3, 4, 5};
//     int max = findMax(arr, LEN);
//     printf("该数组中最大的数是：%d\n", max);
//     return 0;
// }

// int sumArray(int *arr, int len)
// {
//     int sum = 0;
//     for (int i = 0; i < len; i++)
//     {
//         sum += arr[i];
//     }
//     return sum;
// }

// int main(int argc, char const *argv[])
// {
//     /* 在主函数中使用指针动态分配有5个int型变量长度的内存，并将该指针输入进sumArray()函数计算数组元素之和 */
//     int len = 5;
//     int *p = (int *)malloc(len * sizeof(int));
//     if (p == NULL)
//     {
//         printf("内存分配失败。\n");
//         return 1;
//     }
//     // 初始化数组
//     for (int i = 0; i < len; i++)
//     {
//         p[i] = i + 1;
//     }
//     int sum = sumArray(p, len);
//     printf("数组元素之和等于：%d\n", sum);
//     free(p);
//     p = NULL;
//     return 0;
// }

// int calculateAverage(int (*arr)[5], int num)
// {
//     // 计算总数
//     int sum = 0;
//     for (int i = 0; i < num; i++)
//     {
//         sum += (*arr)[i];
//     }
//     // 计算平均数
//     int average = sum / num;
//     return average;
// }

// int main(int argc, char const *argv[])
// {
//     /*
//     使用calculateAverage()函数利用指针计算数组中元素的平均值，
//     该函数的输入参数有两个，一个是数组的指针，另一个是元素的数量，数组初始有5个元素
//     */
//     int arr[5] = {1, 2, 3, 4, 5};
//     int average = calculateAverage(&arr, 5);
//     printf("该整数数组的平均数是：%d\n", average);
//     return 0;
// }

// void reverseArray(int (*arr)[5], int num)
// {
//     int min = 0, max = num - 1;
//     while (min < max)
//     {
//         int t = (*arr)[min];
//         (*arr)[min] = (*arr)[max];
//         (*arr)[max] = t;

//         min++;
//         max--;
//     }
// }

// void reverseArray2(int arr[], int num)
// {
//     int *p = arr, *q = arr + num - 1;
//     while (p < q)
//     {
//         int t = *p;
//         *p = *q;
//         *q = t;
//         p++;
//         q--;
//     }
// }
// int main(int argc, char const *argv[])
// {
//     /*
//     在reverseArray()函数中使用指针实现数组的反转，
//     reverseArray函数有两个输入参数，一个是数组的指针，另外一个是数组中的元素个数
//     */
//     int arr[5] = {1, 2, 3, 4, 5};
//     reverseArray(&arr, 5);
//     // reverseArray2(arr, 5);
//     for (int i = 0; i < 5; i++)
//     {
//         printf("%d\t", arr[i]);
//     }

//     return 0;
// }

// void swap(int *a, int *b)
// {
//     int t = *a;
//     *a = *b;
//     *b = t;
// }
// int main(int argc, char const *argv[])
// {
//     int a = 10, b = 20;
//     swap(&a, &b);
//     printf("a = %d\n", a);
//     printf("b = %d\n", b);
//     return 0;
// }
