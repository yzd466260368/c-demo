#include "menu.h"

#include "charts.h"
#include "import_export.h"
#include "input.h"
#include "stats.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * 定义函数指针类型。
 * 所有具体菜单功能函数都接收 RecordStore * 参数，返回 void。
 */
typedef void (*MenuFunc)(RecordStore *store);

/* 各菜单功能函数声明。static 表示这些函数只在 menu.c 内部使用。 */
static void add_record(RecordStore *store);
static void edit_record(RecordStore *store);
static void delete_record(RecordStore *store);
static void period_statistics(RecordStore *store);
static void expense_statistics(RecordStore *store);
static void income_statistics(RecordStore *store);
static void show_balance(RecordStore *store);
static void show_chart(RecordStore *store);
static void import_data(RecordStore *store);
static void export_data(RecordStore *store);
static void list_records(RecordStore *store);

/* 辅助输出函数声明 */
static void print_one_record(int display_index, const Record *record);
static void print_records_by_indexes(const RecordStore *store, const int *indexes, int count);
static void print_records_original_order(const RecordStore *store);

/*
 * 菜单项结构体。
 * 每个菜单项表示一个“真正要执行的功能”。
 */
typedef struct
{
    int option;       /* 子菜单中的功能编号 */
    const char *name; /* 子菜单中显示的功能名称 */
    MenuFunc func;    /* 该功能对应的处理函数 */
} MenuItem;

/*
 * 菜单分组结构体。
 * 每个分组表示主菜单中的一个大类。
 */
typedef struct
{
    int option;            /* 主菜单中的分类编号 */
    const char *name;      /* 分类名称 */
    const MenuItem *items; /* 分类下面的子菜单数组 */
    int item_count;        /* 子菜单数量 */
} MenuGroup;

/* 计算数组元素个数，避免手动维护数量 */
#define ARRAY_SIZE(arr) ((int)(sizeof(arr) / sizeof((arr)[0])))

/*
 * 记录管理菜单。
 *
 * 新版增加了修改和删除功能：
 * 1. 新增记录：录入一条新的收入/支出记录。
 * 2. 修改记录：选择已有记录序号，然后重新输入一条记录覆盖原记录。
 * 3. 删除记录：选择已有记录序号，确认后删除。
 * 4. 查看记录：支持按照发生时间或金额升序/降序查看。
 */
static const MenuItem record_menu_items[] = {
    {1, "新增记录", add_record},
    {2, "修改记录", edit_record},
    {3, "删除记录", delete_record},
    {4, "查看记录", list_records}};

/* 统计查询菜单 */
static const MenuItem stats_menu_items[] = {
    {1, "按日/周/月统计收支", period_statistics},
    {2, "支出分类统计", expense_statistics},
    {3, "收入分类统计", income_statistics},
    {4, "余额查询", show_balance},
    {5, "收支数据图表", show_chart}};

/* 数据管理菜单 */
static const MenuItem data_menu_items[] = {
    {1, "导入记录", import_data},
    // {2, "导出记录和报表", export_data}
};

/* 主菜单分组 */
static const MenuGroup menu_groups[] = {
    {1, "记录管理", record_menu_items, ARRAY_SIZE(record_menu_items)},
    {2, "统计查询", stats_menu_items, ARRAY_SIZE(stats_menu_items)},
    {3, "数据管理", data_menu_items, ARRAY_SIZE(data_menu_items)}};

/* 返回主菜单分组数量 */
static int group_count(void)
{
    return ARRAY_SIZE(menu_groups);
}

/* 返回主菜单中的最大编号 */
static int group_max_option(void)
{
    int i;
    int max_option = 0;

    for (i = 0; i < group_count(); i++)
    {
        if (menu_groups[i].option > max_option)
        {
            max_option = menu_groups[i].option;
        }
    }

    return max_option;
}

/* 返回某个子菜单中的最大编号 */
static int item_max_option(const MenuGroup *group)
{
    int i;
    int max_option = 0;

    if (group == NULL)
    {
        return 0;
    }

    for (i = 0; i < group->item_count; i++)
    {
        if (group->items[i].option > max_option)
        {
            max_option = group->items[i].option;
        }
    }

    return max_option;
}

/* 根据用户输入的编号查找主菜单分组 */
static const MenuGroup *find_group(int choice)
{
    int i;

    for (i = 0; i < group_count(); i++)
    {
        if (menu_groups[i].option == choice)
        {
            return &menu_groups[i];
        }
    }

    return NULL;
}

/* 在某个分组中，根据用户输入的编号查找具体菜单项 */
static const MenuItem *find_item(const MenuGroup *group, int choice)
{
    int i;

    if (group == NULL)
    {
        return NULL;
    }

    for (i = 0; i < group->item_count; i++)
    {
        if (group->items[i].option == choice)
        {
            return &group->items[i];
        }
    }

    return NULL;
}

/* 打印主菜单，只显示功能分类，不直接显示所有功能 */
static void print_main_menu(void)
{
    int i;

    printf("\n========== 收支管理系统 =========="
           "\n");

    for (i = 0; i < group_count(); i++)
    {
        printf("%d. %s\n", menu_groups[i].option, menu_groups[i].name);
    }

    printf("0. 退出系统\n");
}

/* 打印某个分类下面的子菜单 */
static void print_sub_menu(const MenuGroup *group)
{
    int i;

    if (group == NULL)
    {
        return;
    }

    printf("\n========== %s =========="
           "\n",
           group->name);

    for (i = 0; i < group->item_count; i++)
    {
        printf("%d. %s\n", group->items[i].option, group->items[i].name);
    }

    printf("0. 返回上一级\n");
}

/* 执行一个子菜单循环 */
static void run_sub_menu(const MenuGroup *group, RecordStore *store)
{
    int choice;
    const MenuItem *item;

    if (group == NULL || store == NULL)
    {
        return;
    }

    while (1)
    {
        print_sub_menu(group);
        choice = input_menu_choice(item_max_option(group));

        if (choice == 0)
        {
            break;
        }

        item = find_item(group, choice);
        if (item == NULL)
        {
            printf("输入错误，请重新选择。\n");
            continue;
        }

        item->func(store);
        input_pause();
    }
}

/* 菜单主循环 */
void menu_run(RecordStore *store)
{
    int choice;
    const MenuGroup *group;

    if (store == NULL)
    {
        return;
    }

    while (1)
    {
        print_main_menu();
        choice = input_menu_choice(group_max_option());

        if (choice == 0)
        {
            break;
        }

        group = find_group(choice);
        if (group == NULL)
        {
            printf("输入错误，请重新选择。\n");
            continue;
        }

        run_sub_menu(group, store);
    }
}

/*
 * 新增一条收入或支出记录。
 * menu.c 不处理具体输入，只调用 input_record 构建 Record，
 * 然后调用 store_add 保存到动态数组。
 */
static void add_record(RecordStore *store)
{
    Record record;

    if (store == NULL)
    {
        return;
    }

    if (!input_record(&record))
    {
        printf("添加失败，输入记录失败。\n");
        return;
    }

    if (store_add(store, &record))
    {
        printf("添加成功。\n");
    }
    else
    {
        printf("添加失败，内存不足。\n");
    }
}

/*
 * 修改一条已有记录。
 * 步骤：
 * 1. 先输出当前所有记录，方便用户查看序号。
 * 2. 调用 input_record_index 获取要修改的记录下标。
 * 3. 调用 input_record 重新输入一条完整记录。
 * 4. 调用 store_update 覆盖原记录。
 */
static void edit_record(RecordStore *store)
{
    int index;
    Record record;

    if (store == NULL)
    {
        return;
    }

    if (store->count == 0)
    {
        printf("暂无记录，不能修改。\n");
        return;
    }

    printf("\n当前记录如下，请根据序号选择要修改的记录。\n");
    print_records_original_order(store);

    index = input_record_index(store->count);
    if (index < 0)
    {
        printf("修改失败，记录序号错误。\n");
        return;
    }

    printf("请重新输入这条记录的完整信息：\n");
    if (!input_record(&record))
    {
        printf("修改失败，输入记录失败。\n");
        return;
    }

    if (store_update(store, index, &record))
    {
        printf("修改成功。\n");
    }
    else
    {
        printf("修改失败。\n");
    }
}

/*
 * 删除一条已有记录。
 * 步骤：
 * 1. 先输出当前所有记录，方便用户查看序号。
 * 2. 调用 input_record_index 获取要删除的记录下标。
 * 3. 调用 input_confirm_delete 二次确认。
 * 4. 调用 store_delete 删除记录。
 */
static void delete_record(RecordStore *store)
{
    int index;

    if (store == NULL)
    {
        return;
    }

    if (store->count == 0)
    {
        printf("暂无记录，不能删除。\n");
        return;
    }

    printf("\n当前记录如下，请根据序号选择要删除的记录。\n");
    print_records_original_order(store);

    index = input_record_index(store->count);
    if (index < 0)
    {
        printf("删除失败，记录序号错误。\n");
        return;
    }

    printf("你选择删除的是：\n");
    print_one_record(index + 1, &store->items[index]);

    if (!input_confirm_delete())
    {
        printf("已取消删除。\n");
        return;
    }

    if (store_delete(store, index))
    {
        printf("删除成功。\n");
    }
    else
    {
        printf("删除失败。\n");
    }
}

/* 按日/周/月统计收支 */
static void period_statistics(RecordStore *store)
{
    PeriodInput period;
    DateTime start_dt;
    DateTime end_dt;
    char start_text[DATETIME_LEN];
    char end_text[DATETIME_LEN];
    PeriodStats result;

    if (store == NULL)
    {
        return;
    }

    if (!input_period_range(&period))
    {
        return;
    }

    time_t_to_datetime(period.start_time, &start_dt);
    time_t_to_datetime(period.end_time, &end_dt);
    format_datetime(&start_dt, start_text, sizeof(start_text));
    format_datetime(&end_dt, end_text, sizeof(end_text));

    printf("统计范围：[%s, %s)\n", start_text, end_text);

    stats_period(store, period.start_time, period.end_time, &result);
    stats_print_period(&result);
}

/* 支出分类统计 */
static void expense_statistics(RecordStore *store)
{
    CategoryStatsList result;

    if (store == NULL)
    {
        return;
    }

    stats_category(store, REC_EXPENSE, &result);
    stats_print_category(&result, "支出分类统计");
}

/* 收入分类统计 */
static void income_statistics(RecordStore *store)
{
    CategoryStatsList result;

    if (store == NULL)
    {
        return;
    }

    stats_category(store, REC_INCOME, &result);
    stats_print_category(&result, "收入分类统计");
}

/* 查询当前总收入、总支出和余额 */
static void show_balance(RecordStore *store)
{
    if (store == NULL)
    {
        return;
    }

    printf("收入总额：%.2f\n", stats_total_income(store));
    printf("支出总额：%.2f\n", stats_total_expense(store));
    printf("当前余额：%.2f\n", stats_balance(store));
}

/* 显示简单星号图表 */
static void show_chart(RecordStore *store)
{
    RecordType type;
    CategoryStatsList result;
    const char *title;

    if (store == NULL)
    {
        return;
    }

    type = input_chart_record_type();
    stats_category(store, type, &result);

    if (type == REC_INCOME)
    {
        title = "收入分类图表";
    }
    else
    {
        title = "支出分类图表";
    }

    chart_category_bar(&result, title);
}

/* 从 CSV 文件导入记录 */
static void import_data(RecordStore *store)
{
    char path[256];
    int count = 0;

    if (store == NULL)
    {
        return;
    }

    if (!input_import_path(path, sizeof(path)))
    {
        return;
    }

    if (import_records_csv(path, store, &count))
    {
        printf("导入完成，成功导入 %d 条记录。\n", count);
    }
    else
    {
        printf("导入失败。\n");
    }
}

/* 导出记录和报表 */
static void export_data(RecordStore *store)
{
    char csv_path[256];
    char report_path[256];
    int ok1;
    int ok2;

    if (store == NULL)
    {
        return;
    }

    if (!input_export_paths(csv_path, sizeof(csv_path), report_path, sizeof(report_path)))
    {
        return;
    }

    ok1 = export_records_csv(csv_path, store);
    ok2 = export_report_txt(report_path, store);

    if (ok1 && ok2)
    {
        printf("导出成功。\n");
    }
    else
    {
        printf("导出失败，请检查路径。\n");
    }
}

/*
 * 比较两条记录。
 * 返回值小于 0：a 应该排在 b 前面。
 * 返回值大于 0：a 应该排在 b 后面。
 */
static int compare_record(const Record *a, const Record *b, const ListInput *option)
{
    int result = 0;
    time_t ta;
    time_t tb;

    if (a == NULL || b == NULL || option == NULL)
    {
        return 0;
    }

    if (option->field == LIST_SORT_BY_AMOUNT)
    {
        if (a->amount < b->amount)
        {
            result = -1;
        }
        else if (a->amount > b->amount)
        {
            result = 1;
        }
        else
        {
            result = 0;
        }
    }
    else
    {
        ta = datetime_to_time_t(&a->time);
        tb = datetime_to_time_t(&b->time);

        if (ta < tb)
        {
            result = -1;
        }
        else if (ta > tb)
        {
            result = 1;
        }
        else
        {
            result = 0;
        }
    }

    if (option->order == LIST_ORDER_DESC)
    {
        result = -result;
    }

    return result;
}

/*
 * 对记录下标数组排序。
 *
 * 这里不直接调整 store->items 的顺序，避免“查看记录”改变真实数据顺序。
 * 只排序 indexes 数组，然后按照 indexes 指向的顺序打印记录。
 */
static void sort_record_indexes(const RecordStore *store, int *indexes, int count, const ListInput *option)
{
    int i;
    int j;
    int best;
    int temp;

    if (store == NULL || indexes == NULL || option == NULL)
    {
        return;
    }

    for (i = 0; i < count - 1; i++)
    {
        best = i;

        for (j = i + 1; j < count; j++)
        {
            if (compare_record(&store->items[indexes[j]], &store->items[indexes[best]], option) < 0)
            {
                best = j;
            }
        }

        if (best != i)
        {
            temp = indexes[i];
            indexes[i] = indexes[best];
            indexes[best] = temp;
        }
    }
}

/* 打印一条记录 */
static void print_one_record(int display_index, const Record *record)
{
    char time_text[DATETIME_LEN];

    if (record == NULL)
    {
        return;
    }

    format_datetime(&record->time, time_text, sizeof(time_text));

    printf("%-6d %-8s %-12.2f %-20s %-12s %s\n",
           display_index,
           record_type_to_text(record->type),
           record->amount,
           time_text,
           record->category,
           record->note);
}

/* 按给定下标顺序打印多条记录 */
static void print_records_by_indexes(const RecordStore *store, const int *indexes, int count)
{
    int i;
    int index;

    if (store == NULL || indexes == NULL)
    {
        return;
    }

    printf("\n%-6s %-8s %-12s %-20s %-12s %s\n", "序号", "类型", "金额", "时间", "分类", "备注");
    printf("----------------------------------------------------------------------------\n");

    for (i = 0; i < count; i++)
    {
        index = indexes[i];
        print_one_record(index + 1, &store->items[index]);
    }
}

/* 按原始数组顺序打印记录，主要用于修改和删除时选择序号 */
static void print_records_original_order(const RecordStore *store)
{
    int i;
    int *indexes;

    if (store == NULL || store->count <= 0)
    {
        return;
    }

    indexes = (int *)malloc(sizeof(int) * store->count);
    if (indexes == NULL)
    {
        printf("内存不足，无法显示记录。\n");
        return;
    }

    for (i = 0; i < store->count; i++)
    {
        indexes[i] = i;
    }

    print_records_by_indexes(store, indexes, store->count);
    free(indexes);
}

/*
 * 查看全部记录。
 *
 * 用户可以选择：
 * 1. 按发生时间排序，升序或降序。
 * 2. 按金额排序，升序或降序。
 *
 * 这里排序的是下标数组，不会改变真实保存顺序。
 */
static void list_records(RecordStore *store)
{
    int i;
    int *indexes;
    ListInput option;

    if (store == NULL)
    {
        return;
    }

    if (store->count == 0)
    {
        printf("暂无记录。\n");
        return;
    }

    if (!input_list_option(&option))
    {
        printf("查看失败，排序条件输入错误。\n");
        return;
    }

    indexes = (int *)malloc(sizeof(int) * store->count);
    if (indexes == NULL)
    {
        printf("内存不足，无法显示记录。\n");
        return;
    }

    for (i = 0; i < store->count; i++)
    {
        indexes[i] = i;
    }

    sort_record_indexes(store, indexes, store->count, &option);
    print_records_by_indexes(store, indexes, store->count);

    free(indexes);
}
