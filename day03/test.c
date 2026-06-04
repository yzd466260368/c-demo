#include <stdio.h>

int main(int argc, char const *argv[])
{
    /*
    使用scanf为一个有10个元素的数组初始化0~10的数字，统计其中每个元素出现的次数
    */
    int arr[10];
    int count[11] = {0};

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        scanf("%d", &arr[i]);
        count[arr[i]]++;
    }
    for (int i = 0; i < sizeof(count) / sizeof(count[0]); i++)
    {
        printf("数字%d：%d次\n", i, count[i]);
    }

    return 0;
}

#include <stdbool.h>
#define PI 3.14159

/*
查找一个有5个元素的数组中的第二大的元素
 */
// int arr[5] = {5, 6, 7, 8, 3};

// for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
// {
//     for (int j = i + 1; j < sizeof(arr) / sizeof(arr[0]); j++)
//     {
//         if (arr[j] < arr[i])
//         {
//             int temp = arr[j];
//             arr[j] = arr[i];
//             arr[i] = temp;
//         }
//     }
// }

// // for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
// // {
// //     printf("%d ", arr[i]);
// // }
// printf("第二大的元素是：%d", arr[(sizeof(arr) / sizeof(arr[0]) - 2)]);

/*
初始化一个有10个元素的数组，计算输出数组中所有偶数的个数
 */
// int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
// int count = 0;

// for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
// {
//     if (arr[i] % 2 == 0)
//     {
//         count++;
//     }
// }
// printf("偶数的个数是：%d", count);

/*
判断一个有5个元素的整数数组是否是升序排列的
*/
// int arr[5] = {1, 3, 2, 4, 5};
// int flag = 0;

// for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
// {
//     for (int j = i + 1; j < sizeof(arr) / sizeof(arr[0]); j++)
//     {
//         if (!(arr[i] <= arr[j]))
//         {
//             flag = 1;
//         }
//     }
// }
// if (flag == 0)
// {
//     printf("这是一个升序数组");
// }
// else
// {
//     printf("这不是一个升序数组");
// }

/*
反转一个有5个元素的数组
*/
// int arr[5] = {1, 2, 3, 4, 5};

// for (int max_index = (sizeof(arr) / sizeof(arr[0])) - 1, min_index = 0; max_index > min_index; min_index++, max_index--)
// {
//     int temp = arr[max_index];
//     arr[max_index] = arr[min_index];
//     arr[min_index] = temp;
// }

// for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
// {
//     printf("%d ", arr[i]);
// }

/*

    找出一个数组中的最大值和最小值。数组有5个元素

    */
// int arr[] = {5, 4, 3, 2, 1};
// int max = arr[0], min = arr[0];
// for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
// {
//     if (arr[i] > max)
//     {
//         max = arr[i];
//     }
//     if (arr[i] < min)
//     {
//         min = arr[i];
//     }
// }
// printf("数组中最大值%d，最小值%d", max, min);

/*
利用循环结构向一个数组中输入10个整数，并输出这些整数的和
*/
// int arr[10] = {0};
// int sum = 0;
// for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
// {
//     int temp;
//     scanf("%d", &temp);
//     sum += temp;
// }
// printf("%d", sum);

// char str[] = "";
// if (str[0] == '\0')
// {
//     printf("11111");
// }
// int arr[3][2] = {{1, 2}, {3, 4}, {5, 6}};
// // arr 代表数组首地址（首元素arr[0][0]地址），不管转不转型，ptr 都指向 1 的地址
// int *ptr = (int *)arr; // 强制转换为指针类型
// // int *ptr = arr; // 报错， incompatible pointer type 'int (*)[2]， 类型不匹配
// printf("%d", *(ptr + 3));

/*
int(*p)[N] → 行指针，一次跳一整行（2 个 int）
int(*p)[N] 里的 N = 二维数组的列数
int *ptr → 元素指针，一次只跳 1 个 int
*/
// int arr[3][3] = {{1, 2, 7}, {3, 4, 8}, {5, 6, 9}};
// int (*p)[3] = arr;
// // 遍历行
// for (int i = 0; i < 3; i++)
// {
//     // 遍历当前行每一列
//     for (int j = 0; j < 3; j++)
//     {
//         // (*(p + i))[j] = arr[i][j]
//         printf("%d ", *(*(p + i) + j));
//     }
//     putchar('\n');
// }
// int arr[2][2] = {{1, 2}, {3, 4}};
// /*
//     *arr[0] → arr[0][0] = 1
//     *arr[1] → arr[1][0] = 3
//  */
// printf("%d", *arr[2]);

// int arr[3][2] = {{1, 2}, {3, 4}, {5, 6}};
// int *ptr = arr[1];
// printf("%d", *(ptr + 1));

// int arr[] = {1, 2, 3, 5, 6};
// int *p = &arr[0];
// printf("%d", *(p + 1));

// int a = ~5;
// printf("%d", a);
// char c1 = 'b', c2 = 'e';
// printf("%d,%c\n", c2 - c1, c2 - 'a' + 'A');
// int c1 = 1, c2 = 2, c3 = 2;
// c3 = c1 / c2 * c3;
// printf("%d\n", c3);
// int a, b;
// a *b;

// double c = 0.123;
// double d = ++c;
// printf("%lf", d);

/*
周长：C=2πr
面积：S=πr2
示例输入：
'radius = 7'
示例输出：
'Area = 153.938, Circumference = 43.982'
*/
// double r;
// printf("请输入半径\n");
// scanf("%lf", &r);
// double Circumference = 2 * PI * r;
// double Area = PI * r * r;
// printf("Area = %.3lf, Circumference = %.3lf", Area, Circumference);

/*
示例输入：
length = 5, width = 3
示例输出：
Area = 15, Perimeter = 16
*/
// int length, width;
// printf("请输入长宽\n");
// scanf("%d %d", &length, &width);
// printf("length = %d, width = %d", length, width);
// printf("Area = %d, Perimeter = %d", (length * width), (2 * (length + width)));

/*
示例输入：
num = 2  // 二进制表示：0010
示例输出：
num = 10  // 二进制表示：1010
*/
// int num;
// printf("请输入一个整数\n");
// scanf("%d", &num);
// num = num | (1 << 3);
// printf("num = %d", num);

/*
清除一个整数的位3（从0开始），用左移运算符和位运算符将该位置的位设置为0
*/
// int num;
// printf("请输入一个整数\n");
// scanf("%d", &num);
// num = num & (~(1 << 3));
// printf("num = %d", num);

/*
判断一个整数的位n（从位0开始）是否为1， 使用位运算
*/
// int num, n;
// printf("请输入整数和位数\n");
// scanf("%d %d", &num, &n);

// if ((num & (1 << n)) != 0)
// {
//     printf("第%d位是1\n", n);
// }
// else
// {
//     printf("第%d位不是1\n", n);
// }

// int a, b;
// if (a > b)
//     ;

// int num;
// scanf("%d", &num);
// switch (num)
// {
// case true:
//     printf("true执行了");
//     break;
// case false:
//     printf("false执行了");
//     break;

// default:
//     printf("default执行了");
//     break;
// }

// int x = 2;

// switch (x)
// {
// case 1:
//     printf("One");
// case 2:
//     printf("Two");
// case 3:
//     printf("Three");
//     break;
// default:
//     printf("Other");
// }

// int a = 0, b = 3;
// switch (a + b && a && b++ && a++)
// {
// case 0:
//     a++;
// case 1:
//     b++;
// case 2:
//     a += b;
//     break;
// case 3:
//     a -= b;
// default:
//     a;
// }
// printf("%d", a);

/*
    从小到大输出这3个整数
    */
// int x, y, z;
// printf("请输入3个整数\n");
// scanf("%d %d %d", &x, &y, &z);
// if (x < y)
// {
//     // x小
//     if (x < z)
//     {
//         if (y < z)
//         {
//             printf("%d<=%d<=%d", x, y, z);
//         }
//         else
//         {
//             printf("%d<=%d<=%d", x, z, y);
//         }
//     }
//     else
//     {
//         printf("%d<=%d<=%d", z, x, y);
//     }
// }
// else
// {
//     // y小
//     if (y < z)
//     {
//         if (x < z)
//         {
//             printf("%d<=%d<=%d", y, x, z);
//         }
//         else
//         {
//             printf("%d<=%d<=%d", y, z, x);
//         }
//     }
//     else
//     {
//         printf("%d<=%d<=%d", z, y, x);
//     }
// }

// int x;
// printf("请输入一个整数\n");
// scanf("%d", &x);
// if (x > 0)
// {
//     printf("%d是一个正整数", x);
// }
// else if (x < 0)
// {
//     printf("%d是一个负整数", x);
// }
// else
// {
//     printf("%d是一个0", x);
// }

/*
判断是否是闰年
*/
// int year;
// printf("请输入一个年份\n");
// scanf("%d", &year);

// /*
// 能被 4 整除，但不能被 100 整除
// 能被 400 整除
// */

// if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
// {
//     printf("%d是一个闰年", year);
// }
// else
// {
//     printf("%d不是一个闰年", year);
// }

/*
输入一个整数，判断是否是偶数
*/
// int num;
// printf("请输入一个整数\n");
// scanf("%d", &num);
// if (num % 2 == 0)
// {
//     printf("%d是一个偶数", num);
// }
// else
// {
//     printf("%d不是一个偶数", num);
// }

/*
输入一个数，判断是否是5或者7的倍数
*/
// int num;
// printf("输入一个数\n");
// scanf("%d", &num);
// if (num % 5 == 0 || num % 7 == 0)
// {
//     printf("%d这个数是5或者7的倍数", num);
// }
// else
// {
//     printf("%d这个数不是5或者7的倍数", num);
// }

/*
输入两个数，输出较大的数
*/
// int a, b;
// scanf("%d %d", &a, &b);
// if (a > b)
// {
//     printf("较大的数是%d", a);
// }
// else
// {
//     printf("较大的数是%d", b);
// }

/*
输入一个月份数字，输出对应的季节，12-1-2为winter， 后面依次是spring，summer， autumn
*/

// int month;
// printf("请输入一个月份\n");
// scanf("%d", &month);
// switch (month)
// {
// case 12:
// case 1:
// case 2:
//     printf("Winter\n");
//     break;

// case 3:
// case 4:
// case 5:
//     printf("Spring\n");
//     break;
// case 6:
// case 7:
// case 8:
//     printf("Summer\n");
//     break;
// case 9:
// case 10:
// case 11:
//     printf("Autumn\n");
//     break;

// default:
//     printf("月份出错\n");
//     break;
// }

/*
输入简化后的代码
*/
// int x = 7;

// if (x > 0 && x < 10)
// {
//     printf("x is between 0 and 10\n");
// }

/*
    用户输入两个数和一个操作符（+ - * /），根据操作符进行相应的运算，并输出结果
    使用 switch 语句处理不同的操作符情况，并检查除法操作是否有零作为除数。
     */
// int a, b;
// char symbol;
// printf("请输入两个整数和一个操作符\n");
// scanf("%d %d %c", &a, &b, &symbol);
// switch (symbol)
// {
// case '+':
//     printf("%d + %d = %d", a, b, (a + b));
//     break;

// case '-':
//     printf("%d - %d = %d", a, b, (a - b));
//     break;

// case '*':
//     printf("%d * %d = %d", a, b, (a * b));
//     break;

// case '/':
//     if (b != 0)
//     {
//         printf("%d / %d = %d", a, b, (a / b));
//     }
//     else
//     {
//         printf("除数不能为0");
//     }

//     break;

// default:
//     printf("不支持的符号类型。\n");
//     break;
// }

/*

用户输入一个字符， 判断它是字母，大写字母，小写字母，数字还是其他字符
可以使用 if-else 结构和字符的 ASCII 码范围。
*/
// char ch;
// printf("请输入一个字符\n");
// scanf("%c", &ch);
// if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
// {
//     printf("%c是一个字母\n", ch);
//     if (ch >= 'A' && ch <= 'Z')
//     {
//         printf("%c是一个大写字母\n", ch);
//     }
//     else
//     {
//         printf("%c是一个小写字母\n", ch);
//     }
// }
// else if (ch >= '0' && ch <= '9')
// {
//     printf("%c是一个数字\n", ch);
// }
// else
// {
//     printf("%c是其他字符\n", ch);
// }

// for (int i = 1; i <= 3; i++)
// {
//     for (int j = 1; j <= 2; j++)
//     {
//         if (j == 2)
//         {
//             break;
//         }
//         printf("%d-%d ", i, j);
//     }
// }
// int c = 0;
// while (1 + 2)
// {
//     printf("%d", c);
//     if (c == 5)
//     {
//         break;
//     }

//     c++;
// }

// for (1 + 2; 0 < 1;)
// {
//     /* code */
// }
// int x = 23;
// do
// {
//     printf("%2d", x--);
// } while (!x);

// int x = -1;
// do
// {
//     x = x * x;
//     printf("%d", x);
// } while (!x);

/*
输出1-100之间能被3整除的数字
*/
// for (int i = 1; i <= 100; i++)
// {
//     if (i % 3 == 0)
//     {
//         printf("%d ", i);
//     }
// }

/*
打印99乘法表
*/
// for (int i = 1; i <= 9; i++)
// {
//     for (int j = 1; j <= i; j++)
//     {
//         printf("%d * %d = %d \t", i, j, (i * j));
//     }
//     printf("\n");
// }

/*
打印1-100所有奇数之和
*/
// int sum = 0;
// for (int i = 1; i <= 100; i++)
// {
//     if (!(i % 2 == 0))
//     {
//         sum += i;
//     }
// }
// printf("1-100奇数之和等于%d", sum);

/*
计算一个数字的阶乘 6*5*4*3*2*1
*/
// int num;
// long fact = 1; // 阶乘结果，用long防止溢出
// printf("请输入一个数\n");
// scanf("%d", &num);
// for (int i = num; i > 1; i--)
// {
//     fact *= i;
// }
// printf("%d! = %ld\n", num, fact);

/*
输出1到100之间能被5整除但不能被3整除的数字
*/
// for (int i = 1; i <= 100; i++)
// {
//     if (i % 5 == 0 && i % 3 != 0)
//     {
//         printf("%d ", i);
//     }
// }

/*
输出一个数字的反向数字(不是字符)，即输入12345， 输出54321
*/
// long long int a;
// scanf("%lld", &a);
// while (a != 0)
// {
//     printf("%lld", (a % 10));
//     a /= 10;
// }

/*
输出所有两位数的数字中，十位和个位的和为8的数字
*/

// for (int i = 10; i <= 99; i++)
// {
//     if ((i % 10) + (i / 10) == 8)
//     {
//         printf("%d ", i);
//     }
// }