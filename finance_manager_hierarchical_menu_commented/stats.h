#ifndef STATS_H
#define STATS_H

/*
 * stats.h
 *
 * 本模块负责所有统计功能。
 * 统计模块只负责计算和输出统计结果，不负责菜单输入，也不负责文件保存。
 */

#include "model.h"

/* 分类统计最多保存 100 个分类，通常已经足够课程设计使用 */
#define MAX_CATEGORY_STATS 100

/* 日/周/月区间统计结果 */
typedef struct {
    double income;       /* 收入总额 */
    double expense;      /* 支出总额 */
    double balance;      /* 净余额 = 收入 - 支出 */
    int income_count;    /* 收入笔数 */
    int expense_count;   /* 支出笔数 */
} PeriodStats;

/* 单个分类的统计结果 */
typedef struct {
    char category[CATEGORY_LEN];
    double amount;
    int count;
} CategoryStat;

/* 多个分类的统计结果 */
typedef struct {
    CategoryStat items[MAX_CATEGORY_STATS];
    int count;
    double total;
} CategoryStatsList;

void stats_period(const RecordStore *store, time_t start_time, time_t end_time, PeriodStats *result);
double stats_total_income(const RecordStore *store);
double stats_total_expense(const RecordStore *store);
double stats_balance(const RecordStore *store);
void stats_category(const RecordStore *store, RecordType type, CategoryStatsList *result);
void stats_print_period(const PeriodStats *stats);
void stats_print_category(const CategoryStatsList *stats, const char *title);

#endif
