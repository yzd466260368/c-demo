#include <stdio.h>

int main(int argc, char const *argv[])
{
    printf("\033[2J");      // 清屏
    printf("\033[H");       // 光标回到左上角
    printf("\033[10;20H@"); // 在第10行20列画一个@
    printf("\033[?25l");    // 隐藏光标
    printf("\033[?25h");    // 显示光标
    return 0;
}
