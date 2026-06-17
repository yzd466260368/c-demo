#include <stdio.h>
#include <string.h>

struct Person
{
    char name[10];
    int age;
};
struct Point
{
    int x;
    int y;
};
struct
{
    int y;
    int x;
} Point;

struct
{
    float salary;
    char name[20];
} employee = {50000.0, "Alice"};

union test2
{
    float a;
    float b;
};

int main(int argc, char const *argv[])
{
    // struct Person p1;
    // p1.age = 10;
    // strcpy(p1.name, "p1");
    // printf("%s", p1.name);
    struct Point p = {3, 4};
    printf("%ld\n", sizeof(struct Point));

    struct Point p1 = p;
    printf("%d\n", p1.x);

    struct Point *p2 = &p;
    printf("%d\n", (*p2).y);

    Point.x = 1;

    printf("%d\n", Point.x);

    printf("%.2lf\n", employee.salary);

    union test2 t = {3.14};

    printf("%.2f\n", t.b);

    enum day
    {
        Sunday = 5,
        Monday,
        Tuesday,
        Wednesday
    };
    printf("%d\n", Wednesday);

    enum sizes
    {
        Small = 1,
        Medium = 5,
        Large
    };
    printf("%d", Large);

    return 0;
}

/*
使用scanf为一个有10个元素的数组初始化0~10的数字，统计其中每个元素出现的次数
*/
// int arr[10];
// int count[11] = {0};

// for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
// {
//     scanf("%d", &arr[i]);
//     count[arr[i]]++;
// }
// for (int i = 0; i < sizeof(count) / sizeof(count[0]); i++)
// {
//     printf("数字%d：%d次\n", i, count[i]);
// }

// int a[5] = {10, 20, 30, 40, 50};
// printf("%p\n", &a[0]);
// printf("%p\n", &a[1]);
// printf("%p\n", &a[2]);
// printf("%p\n", &a[3]);
// printf("%p\n", &a[4]);
// int *o = &a[3];
// printf("%d\n", *o);

// printf("%d\n", a[0]);
// int b = 10;
// int *p = &b;
// printf("%p\n", p);  // p存的就是地址，%p打印的就是地址值， 00000070B4BFFA0C
// printf("%p\n", *p); // %p需要传入一个地址，但是传了个10，导致解析成一个十六进制的数，000000000000000A
// printf("%d\n", *p); // 解引用，获取到p地址的值 10

/*
使用scanf为一个有5个元素的数组初始化，判断数组是否包含有重复元素
*/
// int arr[5];
// printf("请输入5个元素\n");
// for (int i = 0; i < 5; i++)
// {
//     scanf("%d", &arr[i]);
// }
// printf("数组初始化完成\n");
// int flag = 0;
// for (int i = 0; i < 5; i++)
// {
//     for (int j = i + 1; j < 5; j++)
//     {
//         if (arr[i] == arr[j])
//         {
//             flag = 1;
//             break;
//         }
//     }
// }
// if (flag == 1)
// {
//     printf("数组有重复元素");
// }
// else
// {
//     printf("数组没有重复元素");
// }

/*
    使用scanf初始化具有10个整型元素的数组，计算数组中所有奇数的和
*/
// int arr[10];
// printf("请输入10个整数\n");
// int sum = 0;
// for (int i = 0; i < 10; i++)
// {
//     scanf("%d", &arr[i]);
//     if (arr[i] % 2 != 0)
//     {
//         sum += arr[i];
//     }
// }
// printf("数组中所有奇数的和是：%d\n", sum);

/*
输入一个3*3的矩阵，并输出该矩阵的转置矩阵（即行列互换）
*/
// int arr[3][3];
// printf("请输入一个矩阵\n");
// // for (int i = 0; i < 3; i++)
// // {
// //     for (int j = 0; j < 3; j++)
// //     {
// //         printf("请输入矩阵第%d行第%d列的数据\n", i, j);
// //         scanf("%d", &arr[i][j]);
// //     }
// // }
// // 二维数组内存连续
// for (int i = 0; i < 3 * 3; i++)
// {
//     scanf("%d", *arr + i);
// }
// for (int i = 0; i < 3; i++)
// {
//     for (int j = 0; j < 3; j++)
//     {
//         if (j > i)
//         {
//             int t = *(*(arr + i) + j);
//             *(*(arr + i) + j) = *(*(arr + j) + i);
//             *(*(arr + j) + i) = t;
//         }
//     }
// }

// for (int i = 0; i < 3; i++)
// {
//     for (int j = 0; j < 3; j++)
//     {
//         printf("%d ", *(*(arr + i) + j));
//     }
//     putchar('\n');
// }

/*

计算一个5*5矩阵的主对角线元素之和
*/
// int arr[5][5];

// printf("请输入一个5*5的矩阵\n");
// for (int i = 0; i < 5 * 5; i++)
// {
//     scanf("%d", *arr + i);
// }
// int sum = 0;
// for (int i = 0; i < 5; i++)
// {
//     for (int j = 0; j < 5; j++)
//     {
//         if (i == j)
//         {
//             sum += *((*arr + i) + j);
//         }
//     }
// }
// printf("5*5矩阵的主对角线元素之和是：%d", sum);

/*
查找并输出一个4*4矩阵中最大元素及其位置
*/

// int x, y;    // 位置
// int max = 0; // 最大值

// int arr[4][4];

// printf("请输入一个4*4的数组\n");
// for (int i = 0; i < 4 * 4; i++)
// {
//     scanf("%d", *arr + i); // 易错：*arr + i 和 *(arr + i) 不等价，(arr + i)移动了行指针， *arr指向 arr[0][0], (arr + i)相当于arr[0+i][0]
// }

// for (int i = 0; i < 4; i++)
// {
//     for (int j = 0; j < 4; j++)
//     {
//         if (*(*(arr + i) + j) > max)
//         {
//             max = *(*(arr + i) + j);
//             x = i;
//             y = j;
//         }
//         printf("%d", *(*(arr + i) + j));
//     }
//     putchar('\n');
// }

// printf("4*4矩阵最大值是：%d，位置在第%d行，第%d列\n", max, x, y);

/*
输入一个3*3的矩阵，并判断该矩阵是否是对称矩阵，（即第i行j列的元素是否等于第j行i列上的元素）
*/
// int arr[3][3];
// printf("请输入一个3*3矩阵\n");
// for (int i = 0; i < 3 * 3; i++)
// {
//     scanf("%d", *arr + i);
// }
// int flag = 0;
// for (int i = 0; i < 3; i++)
// {
//     for (int j = 0; j < 3; j++)
//     {
//         if (j > i)
//         {
//             if (*(*(arr + i) + j) != *(*(arr + j) + i))
//             {
//                 flag = 1;
//             }
//         }
//     }
// }
// if (flag == 1)
// {
//     printf("该矩阵不是一个对称矩阵\n");
// }
// else
// {
//     printf("该矩阵是一个对称矩阵\n");
// }