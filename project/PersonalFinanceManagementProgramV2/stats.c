#include "stats.h"

#include <stdio.h>
#include <string.h>

/*
 * 在已有分类统计结果中查找分类。
 * 找到返回下标，没找到返回 -1。
 */
static int find_category(const CategoryStatsList *list, const char *category) {
    int i;

    if (list == NULL || category == NULL) {
        return -1;
    }

    for (i = 0; i < list->count; i++) {
        if (strcmp(list->items[i].category, category) == 0) {
            return i;
        }
    }

    return -1;
}

/*
 * 按金额从大到小排序分类统计结果。
 * 这里使用简单选择排序，逻辑直观，适合基础 C 实现。
 */
static void sort_category(CategoryStatsList *list) {
    int i;
    int j;
    CategoryStat temp;

    if (list == NULL) {
        return;
    }

    for (i = 0; i < list->count - 1; i++) {
        for (j = i + 1; j < list->count; j++) {
            if (list->items[j].amount > list->items[i].amount) {
                temp = list->items[i];
                list->items[i] = list->items[j];
                list->items[j] = temp;
            }
        }
    }
}

/*
 * 按时间区间统计收入和支出。
 * 步骤：
 * 1. 把结果 result 清零。
 * 2. 遍历所有记录。
 * 3. 把记录时间转换为 time_t，便于比较。
 * 4. 如果时间在 [start_time, end_time) 范围内，就加入统计。
 * 5. 最后计算 balance。
 */
void stats_period(const RecordStore *store, time_t start_time, time_t end_time, PeriodStats *result) {
    int i;
    time_t t;
    const Record *r;

    if (result == NULL) {
        return;
    }

    result->income = 0.0;
    result->expense = 0.0;
    result->balance = 0.0;
    result->income_count = 0;
    result->expense_count = 0;

    if (store == NULL) {
        return;
    }

    for (i = 0; i < store->count; i++) {
        r = &store->items[i];
        t = datetime_to_time_t(&r->time);

        if (t >= start_time && t < end_time) {
            if (r->type == REC_INCOME) {
                result->income += r->amount;
                result->income_count++;
            } else if (r->type == REC_EXPENSE) {
                result->expense += r->amount;
                result->expense_count++;
            }
        }
    }

    result->balance = result->income - result->expense;
}

/* 统计所有收入总额 */
double stats_total_income(const RecordStore *store) {
    int i;
    double total = 0.0;

    if (store == NULL) {
        return 0.0;
    }

    for (i = 0; i < store->count; i++) {
        if (store->items[i].type == REC_INCOME) {
            total += store->items[i].amount;
        }
    }

    return total;
}

/* 统计所有支出总额 */
double stats_total_expense(const RecordStore *store) {
    int i;
    double total = 0.0;

    if (store == NULL) {
        return 0.0;
    }

    for (i = 0; i < store->count; i++) {
        if (store->items[i].type == REC_EXPENSE) {
            total += store->items[i].amount;
        }
    }

    return total;
}

/* 当前余额 = 收入总额 - 支出总额 */
double stats_balance(const RecordStore *store) {
    return stats_total_income(store) - stats_total_expense(store);
}

/*
 * 分类统计。
 * 参数 type 决定统计收入分类还是支出分类。
 * 步骤：
 * 1. 清空 result。
 * 2. 遍历全部记录。
 * 3. 只统计类型匹配的记录。
 * 4. 如果分类已经存在，就累加金额和笔数。
 * 5. 如果分类不存在，就新增一个分类。
 * 6. 最后按金额从大到小排序。
 *
 * store 使用 const，因为统计只读数据，不修改记录。
 */
void stats_category(const RecordStore *store, RecordType type, CategoryStatsList *result) {
    int i;
    int index;
    const Record *r;

    if (result == NULL) {
        return;
    }

    result->count = 0;
    result->total = 0.0;

    if (store == NULL) {
        return;
    }

    for (i = 0; i < store->count; i++) {
        r = &store->items[i];

        if (r->type != type) {
            continue;
        }

        index = find_category(result, r->category);
        if (index == -1) {
            if (result->count >= MAX_CATEGORY_STATS) {
                continue;
            }

            index = result->count;
            strcpy(result->items[index].category, r->category);
            result->items[index].amount = 0.0;
            result->items[index].count = 0;
            result->count++;
        }

        result->items[index].amount += r->amount;
        result->items[index].count++;
        result->total += r->amount;
    }

    sort_category(result);
}

/* 输出日/周/月统计结果 */
void stats_print_period(const PeriodStats *stats) {
    if (stats == NULL) {
        return;
    }

    printf("收入合计：%.2f，笔数：%d\n", stats->income, stats->income_count);
    printf("支出合计：%.2f，笔数：%d\n", stats->expense, stats->expense_count);
    printf("净余额  ：%.2f\n", stats->balance);
}

/*
 * 输出分类统计表。
 * 占比 = 当前分类金额 / 总金额 * 100%。
 */
void stats_print_category(const CategoryStatsList *stats, const char *title) {
    int i;
    double percent;

    if (stats == NULL) {
        return;
    }

    printf("\n%s\n", title == NULL ? "分类统计" : title);
    printf("%-16s %-12s %-8s %-8s\n", "分类", "金额", "笔数", "占比");
    printf("------------------------------------------------\n");

    if (stats->count == 0) {
        printf("暂无数据。\n");
        return;
    }

    for (i = 0; i < stats->count; i++) {
        percent = 0.0;
        if (stats->total > 0.0) {
            percent = stats->items[i].amount * 100.0 / stats->total;
        }

        printf("%-16s %-12.2f %-8d %6.2f%%\n",
               stats->items[i].category,
               stats->items[i].amount,
               stats->items[i].count,
               percent);
    }

    printf("------------------------------------------------\n");
    printf("总计：%.2f\n", stats->total);
}
