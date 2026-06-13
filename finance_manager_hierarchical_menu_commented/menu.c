#include "menu.h"

#include "charts.h"
#include "import_export.h"
#include "input.h"
#include "stats.h"

#include <stdio.h>

/*
 * 定义函数指针类型。
 * 所有具体菜单功能函数都接收 RecordStore * 参数，返回 void。
 */
typedef void (*MenuFunc)(RecordStore *store);

/* 各菜单功能函数声明。static 表示这些函数只在 menu.c 内部使用。 */
static void add_record(RecordStore *store);
static void period_statistics(RecordStore *store);
static void expense_statistics(RecordStore *store);
static void income_statistics(RecordStore *store);
static void show_balance(RecordStore *store);
static void show_chart(RecordStore *store);
static void import_data(RecordStore *store);
static void export_data(RecordStore *store);
static void list_records(RecordStore *store);

/*
 * 菜单项结构体。
 *
 * 每个菜单项表示一个“真正要执行的功能”。
 * 例如：添加记录、余额查询、导入记录等。
 */
typedef struct
{
    int option;       /* 子菜单中的功能编号 */
    const char *name; /* 子菜单中显示的功能名称 */
    MenuFunc func;    /* 该功能对应的处理函数 */
} MenuItem;

/*
 * 菜单分组结构体。
 *
 * 每个分组表示主菜单中的一个大类。
 * 例如：记录管理、统计查询、数据管理。
 *
 * group 内部保存一个 MenuItem 数组，表示该分类下面有哪些具体功能。
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
 * 主要放和“原始记录”相关的功能。
 */
static const MenuItem record_menu_items[] = {
    {1, "记录收入/支出", add_record},
    {2, "查看全部记录", list_records}};

/*
 * 统计查询菜单。
 * 主要放统计、余额、图表等分析类功能。
 */
static const MenuItem stats_menu_items[] = {
    {1, "按日/周/月统计收支", period_statistics},
    {2, "支出分类统计", expense_statistics},
    {3, "收入分类统计", income_statistics},
    {4, "余额查询", show_balance},
    {5, "收支数据图表", show_chart}};

/*
 * 数据管理菜单。
 * 主要放和文件导入、导出相关的功能。
 */
static const MenuItem data_menu_items[] = {
    {1, "导入记录", import_data},
    // {2, "导出记录和报表", export_data}
};

/*
 * 主菜单分组。
 *
 * 用户第一次看到的只有这些大类，界面更简洁。
 */
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

/*
 * 执行一个子菜单循环。
 *
 * 用户进入某个分类后，可以连续执行该分类下的功能，
 * 输入 0 时返回主菜单。
 */
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

/*
 * 菜单主循环。
 *
 * 新版菜单采用两层结构：
 * 1. 主菜单只显示功能分类。
 * 2. 进入分类后再显示具体功能。
 *
 * menu.c 仍然只负责菜单展示和函数指针调度，
 * 具体用户输入仍由 input.c 完成。
 */
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
 * 添加一条收入或支出记录。
 *
 * menu.c 不再逐项输入类型、分类、金额、时间、备注。
 * 这些输入和 Record 构建工作已经交给 input_record 完成。
 *
 * 本函数只做两件事：
 * 1. 调用 input_record 得到一条完整记录。
 * 2. 调用 store_add 保存记录。
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
 * 按日/周/月统计收支。
 *
 * menu.c 不再输入统计级别和日期，也不负责校验日期。
 * input_period_range 会直接返回统计需要的开始时间和结束时间。
 */
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

/* 支出分类统计。没有额外用户输入，直接调用统计模块。 */
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

/* 收入分类统计。没有额外用户输入，直接调用统计模块。 */
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

/* 查询当前总收入、总支出和余额。没有额外用户输入。 */
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

/*
 * 显示简单星号图表。
 *
 * menu.c 不再直接输入“收入图表/支出图表”。
 * input_chart_record_type 会把用户选择转换成 RecordType。
 */
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

/*
 * 从 CSV 文件导入记录。
 *
 * menu.c 不再直接输入路径，路径由 input_import_path 负责获取和校验。
 */
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

/*
 * 导出记录和报表。
 *
 * menu.c 不再直接输入 CSV 路径和 TXT 路径，
 * 两个路径统一由 input_export_paths 负责获取和校验。
 */
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
 * 查看全部记录。
 *
 * 该功能本身没有用户输入，只是把已有数据打印出来。
 * 因此保留在 menu.c 中作为菜单功能的输出逻辑。
 */
static void list_records(RecordStore *store)
{
    int i;
    char time_text[DATETIME_LEN];
    const Record *record;

    if (store == NULL)
    {
        return;
    }

    if (store->count == 0)
    {
        printf("暂无记录。\n");
        return;
    }

    printf("\n%-6s %-8s %-12s %-20s %-12s %s\n", "序号", "类型", "金额", "时间", "分类", "备注");
    printf("----------------------------------------------------------------------------\n");

    for (i = 0; i < store->count; i++)
    {
        record = &store->items[i];
        format_datetime(&record->time, time_text, sizeof(time_text));

        printf("%-6d %-8s %-12.2f %-20s %-12s %s\n",
               i + 1,
               record_type_to_text(record->type),
               record->amount,
               time_text,
               record->category,
               record->note);
    }
}
