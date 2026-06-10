#include <stdio.h>
#include <string.h>
#include "record.h"

static void clearInputBuffer()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

void inputRecord(struct Record *record)
{
    int type;

    printf("请输入类型：1.收入  2.支出：");
    scanf("%d", &type);

    if (type == 1)
        record->recordtype = INCOME;
    else
        record->recordtype = EXPENSES;

    printf("请输入金额：");
    scanf("%lf", &record->amount);

    printf("请输入类别：");
    scanf("%29s", record->category);

    printf("请输入日期 年 月 日：");
    scanf("%d %d %d",
          &record->date.year,
          &record->date.month,
          &record->date.day);

    clearInputBuffer();

    printf("请输入备注：");
    fgets(record->note, sizeof(record->note), stdin);

    record->note[strcspn(record->note, "\n")] = '\0';
}

void printRecord(struct Record record)
{
    printf("%s | %.2f | %s | ",
           record.recordtype == INCOME ? "收入" : "支出",
           record.amount,
           record.category);

    printDate(record.date);

    printf(" | %s\n", record.note);
}