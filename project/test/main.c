#include <stdio.h>

typedef enum
{
    CMD_START = 0,
    CMD_STOP,
    CMD_PAUSE,
    CMD_RESUME,
    CMD_MAX
} Command;

typedef void (*CommandHandler)(void);

void handle_start(void);
void handle_stop(void);
void handle_pause(void);
void handle_resume(void);
void handle_max(void);
void handle_invalid(void);
void handle_command(Command cmd);

/* 函数指针数组 */
CommandHandler handlers[CMD_MAX] = {
    [CMD_START] = handle_start,
    [CMD_STOP] = handle_stop,
    [CMD_PAUSE] = handle_pause,
    [CMD_RESUME] = handle_resume};

int main(int argc, char const *argv[])
{

    handle_command(CMD_START);
    handle_command(CMD_STOP);
    handle_command(CMD_PAUSE);
    handle_command(CMD_RESUME);

    handle_command(100);

    return 0;
}

void handle_start(void)
{
    printf("Start\n");
}

void handle_stop(void)
{
    printf("Stop\n");
}

void handle_pause(void)
{
    printf("Pause\n");
}

void handle_resume(void)
{
    printf("Resume\n");
}

void handle_invalid(void)
{
    printf("Invalid command\n");
}

void handle_command(Command cmd)
{
    if (cmd >= 0 && cmd < CMD_MAX && handlers[cmd] != NULL)
    {
        handlers[cmd]();
    }
    else
    {
        handle_invalid();
    }
}
