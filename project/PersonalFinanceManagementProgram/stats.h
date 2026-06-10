#ifndef STATS_H
#define STATS_H

#include "record_list.h"

void monthlyStatistics(struct RecordList *list);
void expenseCategoryStatistics(struct RecordList *list);
void showBalance(struct RecordList *list);

#endif