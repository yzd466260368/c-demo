#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CATEGORY 50
#define MAX_BAR_WIDTH 20

enum RECORDTYPE
{
    INCOME,  // 收入
    EXPENSES // 支出
};

struct Date
{
    int year;
    int month;
    int day;
};

struct CategoryTotal
{
    char category[30];
    double total;
};

struct Record
{
    enum RECORDTYPE recordtype; // 记录类型，INCOME 收入， EXPENSES 支出
    double amount;              // 金额
    char category[30];          // 类别，比如 餐饮、交通、工资
    struct Date date;           // 日期
    char note[100];             // 备注
};

struct RecordList
{
    struct Record *data; // 动态数组
    int size;            // 当前记录数量
    int capacity;        // 当前容量
};

void initRecordList(struct RecordList *list)
{
    list->size = 0;
    list->capacity = 10;

    list->data = (struct Record *)malloc(sizeof(struct Record) * list->capacity);

    if (list->data == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
}

void addRecord(struct RecordList *list, struct Record record)
{
    if (list->size >= list->capacity)
    {
        list->capacity *= 2;

        struct Record *newData = (struct Record *)realloc(
            list->data,
            sizeof(struct Record) * list->capacity);

        if (newData == NULL)
        {
            printf("扩容失败！\n");
            free(list->data);
            exit(1);
        }

        list->data = newData;
    }

    list->data[list->size] = record;
    list->size++;
}

void printRecords(struct RecordList *list)
{
    int i;

    for (i = 0; i < list->size; i++)
    {
        printf("第 %d 条记录：\n", i + 1);

        printf("类型：%s\n",
               list->data[i].recordtype == INCOME ? "收入" : "支出");

        printf("金额：%.2f\n", list->data[i].amount);

        printf("类别：%s\n", list->data[i].category);

        printf("日期：%04d-%02d-%02d\n",
               list->data[i].date.year,
               list->data[i].date.month,
               list->data[i].date.day);

        printf("备注：%s\n", list->data[i].note);

        printf("--------------------\n");
    }
}

void freeRecordList(struct RecordList *list)
{
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

int findCategory(struct CategoryTotal totals[], int count, char category[])
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (strcmp(totals[i].category, category) == 0)
        {
            return i;
        }
    }

    return -1;
}

int countExpenseByCategory(
    struct Record records[],
    int recordCount,
    struct CategoryTotal totals[])
{
    int totalCount = 0;
    int i;

    for (i = 0; i < recordCount; i++)
    {
        if (records[i].recordtype == EXPENSES)
        {
            int index = findCategory(totals, totalCount, records[i].category);

            if (index == -1)
            {
                strcpy(totals[totalCount].category, records[i].category);
                totals[totalCount].total = records[i].amount;
                totalCount++;
            }
            else
            {
                totals[index].total += records[i].amount;
            }
        }
    }

    return totalCount;
}

void printBarChart(struct CategoryTotal totals[], int count)
{
    int i, j;
    double max = 0;

    for (i = 0; i < count; i++)
    {
        if (totals[i].total > max)
        {
            max = totals[i].total;
        }
    }

    if (max == 0)
    {
        printf("暂无支出数据。\n");
        return;
    }

    printf("\n支出分类统计图\n");
    printf("------------------------------\n");

    for (i = 0; i < count; i++)
    {
        int barLength = (int)(totals[i].total / max * MAX_BAR_WIDTH);

        if (barLength == 0 && totals[i].total > 0)
        {
            barLength = 1;
        }

        printf("%-8s | ", totals[i].category);

        for (j = 0; j < barLength; j++)
        {
            printf("#");
        }

        printf(" %.2f\n", totals[i].total);
    }

    printf("------------------------------\n");
}

int main(int argc, char const *argv[])
{

    /*
    记录收入/支出（类型、金额、时间、备注）
    按月份统计收支
    支出分类统计
    余额查询
    简单数据图表（用*号显示）
    */

    struct Record records[] = {
        {EXPENSES, 25.5, "餐饮", {2026, 6, 9}, "午饭"},
        {EXPENSES, 12.0, "交通", {2026, 6, 9}, "公交"},
        {EXPENSES, 80.0, "购物", {2026, 6, 9}, "买衣服"},
        {EXPENSES, 30.0, "娱乐", {2026, 6, 9}, "电影"},
        {EXPENSES, 20.0, "餐饮", {2026, 6, 10}, "晚饭"},
        {INCOME, 3000.0, "工资", {2026, 6, 10}, "兼职收入"},
        {EXPENSES, 8.0, "交通", {2026, 6, 10}, "地铁"}};

    int recordCount = sizeof(records) / sizeof(records[0]);

    struct CategoryTotal totals[MAX_CATEGORY];

    int categoryCount = countExpenseByCategory(records, recordCount, totals);

    printBarChart(totals, categoryCount);

    return 0;
}
