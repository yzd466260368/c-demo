#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

struct People
{
    int id;
    char name[10];
};

void printPeo(struct People *p)
{
}

int main(int argc, char const *argv[])
{

    return 0;
}

// int gcd(int a, int b)
// {
//     int min = a < b ? a : b;

//     for (int i = min; i >= 1; i--)
//     {
//         if (a % i == 0 && b % i == 0)
//         {
//             return i;
//         }
//     }

//     return 1;
// }

// int lcm(int a, int b)
// {
//     return a * b / gcd(a, b);
// }

// int main(int argc, char const *argv[])
// {
//     /* 最大公因数，也称最大公约数、最大公因子，指两个或多个整数共有约数中最大的一个；
//     最小公倍数是指两个或多个整数公有的倍数叫做它们的公倍数，其中除0以外最小的一个公倍数就叫做这几个整数的最小公倍数。最小公倍数=两整数的乘积÷最大公约数 ，
//     所以怎么求最大公约数是关键。 */

//     int a = 48;
//     int b = 18;

//     printf("最大公约数 = %d\n", gcd(a, b));
//     printf("最小公倍数 = %d\n", lcm(a, b));

//     return 0;
// }

// int *mergeArrays(int arr1[], int n1, int arr2[], int n2)
// {

//     // 分配内存
//     int *merged = (int *)malloc((n1 + n2) * sizeof(int));
//     if (merged == NULL)
//     {
//         printf("内存分配失败！\n");
//         return NULL;
//     }
//     // 初始化索引
//     int i = 0, j = 0, k = 0;

//     /* 双指针合并 */
//     while (i < n1 && j < n2)
//     {
//         if (arr1[i] < arr2[j])
//             merged[k++] = arr1[i++];
//         else
//             merged[k++] = arr2[j++];
//     }

//     /* 剩余元素直接复制 */
//     while (i < n1)
//         merged[k++] = arr1[i++];
//     while (j < n2)
//         merged[k++] = arr2[j++];

//     return merged;
// }

// int main(int argc, char const *argv[])
// {
//     /* 定义一个函数mergeArrays，将两个已排序的数组合并为一个排序后的数组 */
//     int arr1[] = {1, 3, 5};
//     int arr2[] = {2, 4, 6};
//     int n1 = sizeof(arr1) / sizeof(arr1[0]);
//     int n2 = sizeof(arr2) / sizeof(arr2[0]);

//     int *merged = mergeArrays(arr1, n1, arr2, n2);
//     if (merged == NULL)
//         return 1;

//     printf("合并后的数组: ");
//     for (int i = 0; i < n1 + n2; i++)
//     {
//         printf("%d ", merged[i]);
//     }
//     printf("\n");

//     free(merged); // 注意释放内存
//     return 0;
// }

// int countDigits(int num)
// {
//     if (num == 0)
//         return 1;

//     int count = 0;
//     long n = num;
//     if (n < 0)
//         n = -n;
//     while (n > 0)
//     {
//         n /= 10;
//         count++;
//     }
//     return count;
// }

// int main(int argc, char const *argv[])
// {
//     /* 定义一个函数countDigits，计算一个整数中数字的个数 */
//     int a = 123456789;
//     int count = countDigits(a);
//     printf("这个整数中的数字的个数是：%d", count);

//     return 0;
// }

// int sumArray(int *arr, int n)
// {
//     int sum = 0;

//     for (int i = 0; i < n; i++)
//     {
//         sum += arr[i];
//     }

//     return sum;
// }

// int main(int argc, char const *argv[])
// {
//     /* 定义一个函数sumArray，计算一个整数数组的总和 */
//     int arr[] = {1, 2, 3, 4, 5};
//     int n = sizeof(arr) / sizeof(arr[0]);
//     int sum = sumArray(arr, n);
//     printf("数组之和是：%d", sum);
//     return 0;
// }

// void reverse(char *s)
// {
//     int i = 0;
//     int j = strlen(s) - 1;
//     while (i < j)
//     {
//         char temp = s[i];
//         s[i] = s[j];
//         s[j] = temp;
//         i++;
//         j--;
//     }
// }

// int main(int argc, char const *argv[])
// {
//     /*
//     定义一个reverse，反转一个字符串
//     */

//     char arr[] = "321cba";
//     reverse(arr);
//     printf("%s", arr); // 输出：321cba
//     return 0;
// }

// bool isPrime(int a);
// int main(int argc, char const *argv[])
// {
//     /* 定义一个函数isPrime，判断一个整数是否是素数，素数：能被1和自己整除 */
//     int a;
//     printf("请输入一个整数\n");
//     scanf("%d", &a);
//     if (isPrime(a))
//     {
//         printf("这个整数是一个素数");
//     }
//     else
//     {
//         printf("这个整数不是一个素数");
//     }

//     return 0;
// }

// bool isPrime(int num)
// {
//     if (num <= 1)
//         return false;
//     for (int i = 2; i < num; i++)
//     {
//         if (num % i == 0)
//         {
//             return false;
//         }
//     }
//     return true;
// }

// int multiply(int a, int b);
// int main(int argc, char const *argv[])
// {
//     /*定义一个函数multiply，计算并返回两个整数的积*/

//     int c = multiply(2, 3);
//     printf("%d", c);
//     return 0;
// }

// int multiply(int a, int b)
// {
//     return a * b;
// }

// long long factorial(int a);

// int main(int argc, char const *argv[])
// {

//     /*
//     定义一个函数factorial计算一个数的阶乘，并在主函数调用
//     */
//     long long num = factorial(5);
//     printf("%lld", num);
//     return 0;
// }

// long long factorial(int a)
// {
//     if (a <= 1)
//     {
//         return 1;
//     }
//     return a * factorial(a - 1);
// }

// int max(int a, int b);
// int main(int argc, char const *argv[])
// {
//     /* 实现一个max函数，返回两个整数中的较大值 */

//     int maxNum = max(5, 6);
//     printf("%d", maxNum);
//     return 0;
// }
// int max(int a, int b)
// {
//     return a > b ? a : b;
// }

// int main(int argc, char const *argv[])
// {

//     for (int i = 0; i < 10; i++)
//     {
//         for (int j = 0; j < 10; j++)
//         {
//             if (i == 5 || j == 5)
//             {
//                 goto flag;
//             }
//             printf("%d %d\n", i, j);
//         }
//     }
// flag:

//     return 0;
// }

// bool idEven(int num);
// int main(int argc, char const *argv[])
// {
//     /*
//     利用一个函数idEven判断一个整数是否是偶数
//     */
//     int num = 3;
//     if (idEven(num))
//     {
//         printf("num是一个偶数");
//     }
//     else
//     {
//         printf("num不是一个偶数");
//     }

//     return 0;
// }

// bool idEven(int num)
// {
//     return num % 2 == 0;
// }

// int add(int a, int b);
// int main(int argc, char const *argv[])
// {
//     int c = add(1, 2);
//     printf("%d", c);
//     return 0;
// }

// int add(int a, int b)
// {
//     return a + b;
// }
