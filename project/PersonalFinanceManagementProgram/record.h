#ifndef RECORD_H
#define RECORD_H

#include "date.h"

enum RECORDTYPE
{
    INCOME = 1,
    EXPENSES = 2
};

struct Record
{
    enum RECORDTYPE recordtype;
    double amount;
    char category[30];
    struct Date date;
    char note[100];
};

void inputRecord(struct Record *record);
void printRecord(struct Record record);

#endif