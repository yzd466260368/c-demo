#include <stdio.h>

int my_atoi(const char *str)
{
    int result = 0;
    int sign = 1;

    if (str == NULL)
    {
        return 0;
    }

    /* 跳过空白字符 */
    while (*str == ' ')
    {
        str++;
    }

    /* 判断正负号 */
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else if (*str == '+')
    {
        str++;
    }

    /* 转换连续的数字字符 */
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }

    return sign * result;
}

int main(void)
{
    printf("%d\n", my_atoi("123"));        /* 123 */
    printf("%d\n", my_atoi("   -456abc")); /* -456 */
    printf("%d\n", my_atoi("+78"));        /* 78 */
    printf("%d\n", my_atoi("abc123"));     /* 0 */

    return 0;
}