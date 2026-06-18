#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    time_t s = time(NULL);
    // printf("%ld", s / 60 / 60 / 24);

    struct tm *t = localtime(&s);
    printf("%4d-%02d-%02d %02d:%02d:%02d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    return 0;
}
