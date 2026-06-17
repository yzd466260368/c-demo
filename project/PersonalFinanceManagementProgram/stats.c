#include <stdio.h>
#include <string.h>
#include "stats.h"
#include "chart.h"

struct CategoryStat
{
    char category[30];
    double total;
};

void monthlyStatistics(struct RecordList *list)
{
    int year, month;
    double income = 0;
    double expenses = 0;

    printf("请输入要统计的年份和月份：");
    scanf("%d %d", &year, &month);

    for (int i = 0; i < list->size; i++)
    {
        if (isSameMonth(list->data[i].date, year, month))
        {
            if (list->data[i].recordtype == INCOME)
                income += list->data[i].amount;
            else
                expenses += list->data[i].amount;
        }
    }

    printf("\n%d年%d月收支统计：\n", year, month);
    printf("总收入：%.2f\n", income);
    printf("总支出：%.2f\n", expenses);
    printf("结余：%.2f\n", income - expenses);
}

void expenseCategoryStatistics(struct RecordList *list)
{
    struct CategoryStat stats[100];
    int count = 0;

    for (int i = 0; i < list->size; i++)
    {
        if (list->data[i].recordtype == EXPENSES)
        {
            int found = -1;

            for (int j = 0; j < count; j++)
            {
                if (strcmp(stats[j].category, list->data[i].category) == 0)
                {
                    found = j;
                    break;
                }
            }

            if (found == -1)
            {
                strcpy(stats[count].category, list->data[i].category);
                stats[count].total = list->data[i].amount;
                count++;
            }
            else
            {
                stats[found].total += list->data[i].amount;
            }
        }
    }

    if (count == 0)
    {
        printf("暂无支出记录。\n");
        return;
    }

    printf("\n支出分类统计：\n");

    for (int i = 0; i < count; i++)
    {
        printBarChart(stats[i].category, stats[i].total);
    }
}

void showBalance(struct RecordList *list)
{
    double income = 0;
    double expenses = 0;

    for (int i = 0; i < list->size; i++)
    {
        if (list->data[i].recordtype == INCOME)
            income += list->data[i].amount;
        else
            expenses += list->data[i].amount;
    }

    printf("\n余额查询：\n");
    printf("总收入：%.2f\n", income);
    printf("总支出：%.2f\n", expenses);
    printf("当前余额：%.2f\n", income - expenses);
}