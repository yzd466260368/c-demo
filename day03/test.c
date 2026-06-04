#include <stdio.h>

int main(int argc, char const *argv[])
{

    return 0;
}

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