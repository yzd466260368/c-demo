#include<stdio.h>


int hexToDecimal(char hex[]);

int main(int argc, char const *argv[])
{
    
    char hex[100];

    printf("请输入一个十六进制的数。\n");
    scanf("%s", hex);




    int a = hexToDecimal(hex);


    if (a != -1)
    {
        printf("%s十六进制转十进制的结果是: %d", hex, a);    
    }

    return 0;
}


int hexToDecimal(char hex[]) {
    int result = 0;
    int i = 0;



    while (hex[i] != '\0')
    {
        char ch = hex[i];
        int value;

        if (ch >= '0' && ch <= '9')
        {
            value = ch - '0';
        }else if (ch >= 'A' && ch <= 'F')
        {
            value = ch - 'A' + 10;
        }else if (ch >= 'a' && ch <= 'f')
        {
            value = ch - 'a' + 10;
        } else{
            printf("非法字符。\n");
            return -1;
        }
        
        result = result * 16 + value;

        i++;
    }
    return result;
}