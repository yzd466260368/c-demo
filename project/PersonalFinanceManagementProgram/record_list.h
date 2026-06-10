#ifndef RECORD_LIST_H
#define RECORD_LIST_H

#include "record.h"

struct RecordList
{
    struct Record *data;
    int size;
    int capacity;
};

void initRecordList(struct RecordList *list);
void addRecord(struct RecordList *list, struct Record record);
void printAllRecords(struct RecordList *list);
void freeRecordList(struct RecordList *list);

#endif