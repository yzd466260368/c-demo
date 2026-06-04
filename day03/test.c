#include <stdio.h>

int main(int argc, char const *argv[])
{

    return 0;
}

#include <stdbool.h>
#define PI 3.14159

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