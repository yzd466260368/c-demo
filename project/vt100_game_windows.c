// 文件名：vt100_game_windows.c
#include <stdio.h>
#include <windows.h>
#include <conio.h>

// 光标移动函数
void moveCursor(int x, int y)
{
    printf("\033[%d;%dH", y, x); // VT100 ANSI 转义序列
}

// 隐藏/显示光标
void setCursorVisible(int visible)
{
    if (visible)
        printf("\033[?25h"); // 显示光标
    else
        printf("\033[?25l"); // 隐藏光标
    fflush(stdout);
}

// 初始化终端 VT 模式
void enableVTMode()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);
}

// 游戏绘制函数
void draw(int x, int y)
{
    printf("\033[2J"); // 清屏
    printf("\033[H");  // 光标回到左上角
    printf("VT100 Windows Demo\n");
    printf("Use WASD to move, q to quit\n");
    moveCursor(x, y);
    printf("@"); // 小人
    fflush(stdout);
}

int main()
{
    int x = 10, y = 5;
    char ch;

    enableVTMode();
    setCursorVisible(0); // 隐藏光标

    while (1)
    {
        draw(x, y);
        ch = _getch(); // 读取按键，不用回车

        if (ch == 'q')
            break;
        if ((ch == 'w' || ch == 'W') && y > 3)
            y--;
        if ((ch == 's' || ch == 'S') && y < 20)
            y++;
        if ((ch == 'a' || ch == 'A') && x > 1)
            x--;
        if ((ch == 'd' || ch == 'D') && x < 40)
            x++;
    }

    setCursorVisible(1);     // 恢复光标
    printf("\033[2J\033[H"); // 清屏，光标回原位
    return 0;
}