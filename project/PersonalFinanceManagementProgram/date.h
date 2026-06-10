#ifndef DATE_H
#define DATE_H

struct Date
{
    int year;
    int month;
    int day;
};

int isSameMonth(struct Date date, int year, int month);
void printDate(struct Date date);

#endif