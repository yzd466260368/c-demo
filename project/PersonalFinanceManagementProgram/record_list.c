#include <stdio.h>
#include <stdlib.h>
#include "record_list.h"

void initRecordList(struct RecordList *list)
{
    list->size = 0;
    list->capacity = 10;
    list->data = malloc(sizeof(struct Record) * list->capacity);

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

        struct Record *newData =
            realloc(list->data, sizeof(struct Record) * list->capacity);

        if (newData == NULL)
        {
            printf("内存扩容失败！\n");
            exit(1);
        }

        list->data = newData;
    }

    list->data[list->size] = record;
    list->size++;
}

void printAllRecords(struct RecordList *list)
{
    if (list->size == 0)
    {
        printf("暂无记录。\n");
        return;
    }

    for (int i = 0; i < list->size; i++)
    {
        printf("%d. ", i + 1);
        printRecord(list->data[i]);
    }
}

void freeRecordList(struct RecordList *list)
{
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}