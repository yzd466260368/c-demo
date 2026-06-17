#include "charts.h"

#include <stdio.h>

/* 每行最多打印 50 个星号，避免金额太大时图表过长 */
#define MAX_STARS 50

/*
 * 用星号打印分类柱状图。
 * 步骤：
 * 1. 找出最大金额 max_amount。
 * 2. 每个分类的星号数量 = 当前金额 / 最大金额 * MAX_STARS。
 * 3. 如果金额大于 0 但计算结果为 0，则至少打印 1 个星号。
 * 4. 输出分类名、星号和金额。
 */
void chart_category_bar(const CategoryStatsList *stats, const char *title) {
    int i;
    int j;
    int stars;
    double max_amount = 0.0;

    if (stats == NULL) {
        return;
    }

    printf("\n%s\n", title == NULL ? "简单图表" : title);
    printf("每行最多 %d 个 *。\n", MAX_STARS);

    if (stats->count == 0) {
        printf("暂无数据。\n");
        return;
    }

    for (i = 0; i < stats->count; i++) {
        if (stats->items[i].amount > max_amount) {
            max_amount = stats->items[i].amount;
        }
    }

    for (i = 0; i < stats->count; i++) {
        stars = 0;
        if (max_amount > 0.0) {
            stars = (int)(stats->items[i].amount / max_amount * MAX_STARS + 0.5);
        }

        if (stats->items[i].amount > 0.0 && stars == 0) {
            stars = 1;
        }

        printf("%-16s | ", stats->items[i].category);
        for (j = 0; j < stars; j++) {
            putchar('*');
        }
        printf(" %.2f\n", stats->items[i].amount);
    }
}
