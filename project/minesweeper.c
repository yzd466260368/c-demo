#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
void enableVTMode()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hOut, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);
}
#else
void enableVTMode()
{
    // Linux 不需要做任何事
}
#endif

#define ROWS 9
#define COLS 9
#define MINES 10

typedef struct
{
    int isMine;     // 是否为雷
    int isFlag;     // 是否被插旗
    int isRevealed; // 是否已被翻开
    int number;     // 周围雷的数量
} Cell;

// 游戏全局变量
Cell board[ROWS][COLS];
int cursorX = 0, cursorY = 0;
int revealedCount = 0;

// VT100 光标移动
void moveCursor(int x, int y)
{
    printf("\033[%d;%dH", y + 3, x * 2 + 1); // +3 给上方留菜单行
    fflush(stdout);
}

// 隐藏/显示光标
void setCursorVisible(int visible)
{
    if (visible)
        printf("\033[?25h");
    else
        printf("\033[?25l");
}

// 清屏
void clearScreen()
{
    printf("\033[2J\033[H");
}

// 计算周围雷数
int countMines(int r, int c)
{
    int count = 0;
    for (int i = r - 1; i <= r + 1; i++)
        for (int j = c - 1; j <= c + 1; j++)
            if (i >= 0 && i < ROWS && j >= 0 && j < COLS && board[i][j].isMine)
                count++;
    return count;
}

// 回溯翻开空区域
void reveal(int r, int c)
{
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS)
        return;
    if (board[r][c].isRevealed || board[r][c].isFlag)
        return;

    board[r][c].isRevealed = 1;
    revealedCount++;

    if (board[r][c].number == 0)
    { // 空格则递归
        for (int i = r - 1; i <= r + 1; i++)
            for (int j = c - 1; j <= c + 1; j++)
                reveal(i, j);
    }
}

// 初始化游戏
void initBoard()
{
    // 清空
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j] = (Cell){0, 0, 0, 0};

    // 随机生成雷
    srand((unsigned int)time(NULL));
    int placed = 0;
    while (placed < MINES)
    {
        int r = rand() % ROWS;
        int c = rand() % COLS;
        if (!board[r][c].isMine)
        {
            board[r][c].isMine = 1;
            placed++;
        }
    }

    // 计算每格周围雷数
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (!board[i][j].isMine)
                board[i][j].number = countMines(i, j);
}

// 绘制整个棋盘
void drawBoard(int revealAll)
{
    clearScreen();
    printf("AWSD移动光标  J翻开  K插旗  Q退出\n");
    printf("剩余雷数: %d\n", MINES);
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (cursorX == j && cursorY == i)
                printf("\033[7m"); // 光标高亮
            if (revealAll || board[i][j].isRevealed)
            {
                if (board[i][j].isMine)
                    printf("* ");
                else if (board[i][j].number > 0)
                    printf("%d ", board[i][j].number);
                else
                    printf("  ");
            }
            else if (board[i][j].isFlag)
                printf("F ");
            else
                printf("# ");
            if (cursorX == j && cursorY == i)
                printf("\033[0m");
        }
        printf("\n");
    }
}

// 判断胜利
int checkWin()
{
    return revealedCount == ROWS * COLS - MINES;
}

int main()
{
    enableVTMode();
    setCursorVisible(0);

    initBoard();
    drawBoard(0);

    while (1)
    {
        moveCursor(cursorX, cursorY);
        char ch = _getch();
        if (ch == 'q' || ch == 'Q')
            break;
        else if (ch == 'w' || ch == 'W')
        {
            if (cursorY > 0)
                cursorY--;
        }
        else if (ch == 's' || ch == 'S')
        {
            if (cursorY < ROWS - 1)
                cursorY++;
        }
        else if (ch == 'a' || ch == 'A')
        {
            if (cursorX > 0)
                cursorX--;
        }
        else if (ch == 'd' || ch == 'D')
        {
            if (cursorX < COLS - 1)
                cursorX++;
        }
        else if (ch == 'k' || ch == 'K')
        {
            board[cursorY][cursorX].isFlag ^= 1;
        } // 插旗/取消
        else if (ch == 'j' || ch == 'J')
        {
            if (board[cursorY][cursorX].isMine)
            {
                drawBoard(1);
                printf("\n你踩到雷了！游戏结束\n");
                break;
            }
            else
            {
                reveal(cursorY, cursorX);
                if (checkWin())
                {
                    drawBoard(1);
                    printf("\n恭喜你，胜利！\n");
                    break;
                }
            }
        }
        drawBoard(0);
    }

    setCursorVisible(1);
    printf("\033[2J\033[H");
    return 0;
}
