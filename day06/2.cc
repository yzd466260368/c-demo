#include <stdio.h>

// char *my_strcpy(char *dest, const char *src)
// {
//     char *ret = dest; // 保存目标字符串的起始地址，最后返回

//     while (*src != '\0')
//     {
//         *dest = *src;
//         dest++;
//         src++;
//     }

//     *dest = '\0'; // 手动拷贝字符串结束符

//     return ret;
// }

int main(int argc, char const *argv[])
{

    /*
    编程题
    1. 冒泡排序
    2. 字符串转数字
    3. strcpy函数实现
    4. 大小端代码判断
    */
    // 1. 冒泡排序

    // int arr[10] = {1, 4, 5, 6, 8, 8, 9, 9, 2, 2};
    // printf("冒泡排序\n");
    // for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
    // {
    //     for (int j = i + 1; j < sizeof(arr) / sizeof(arr[0]); j++)
    //     {
    //         if (arr[i] > arr[j])
    //         {
    //             int t = *(arr + i);
    //             *(arr + i) = *(arr + j);
    //             *(arr + j) = t;
    //         }
    //     }
    // }
    // printf("排序后的数组\n");
    // for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
    // {
    //     printf("%d\t", *(arr + i));
    // }

    // 2. 字符串转数字
    // 只包含正数
    // char str[6] = "12345";
    // int num = 0;

    // for (int i = 0; str[i] != '\0'; i++)
    // {
    //     // str[i] - '0' 把字符数字转成真正的整数
    //     num = num * 10 + (str[i] - '0');
    // }

    // printf("转换后的数字是：%d\n", num);

    // 包含负数
    // char str[] = "-12345";
    // int num = 0;
    // int sign = 1;
    // int i = 0;

    // if (str[0] == '-')
    // {
    //     sign = -1;
    //     i = 1;
    // }

    // for (; str[i] != '\0'; i++)
    // {
    //     if (str[i] < '0' || str[i] > '9')
    //     {
    //         printf("字符串中包含非数字字符，转换失败\n");
    //         return 1;
    //     }

    //     num = num * 10 + (str[i] - '0');
    // }

    // num = num * sign;

    // printf("转换后的数字是：%d\n", num);

    // 3. strcpy函数实现

    // char src[] = "hello";
    // char dest[20];

    // my_strcpy(dest, src);

    // printf("src = %s\n", src);
    // printf("dest = %s\n", dest);

    // 4. 大小端代码判断
    // int a = 0x12345678;
    // char *p = (char *)&a;
    // if (*p == 0x78)
    // {
    //     printf("%d\n", *p);
    //     printf("该系统是小端序\n");
    // }
    // else
    // {
    //     printf("该系统是大端序\n");
    // }

    int arr[5][5];
    int (*p)[4];
    p = (int (*)[4])arr;
    printf("%d\n", &p[4][2] - &arr[4][2]);

    return 0;
}
