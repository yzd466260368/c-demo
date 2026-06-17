#include <stdio.h>
#include "date.h"

int isSameMonth(struct Date date, int year, int month)
{
    return date.year == year && date.month == month;
}

void printDate(struct Date date)
{
    printf("%04d-%02d-%02d", date.year, date.month, date.day);
}