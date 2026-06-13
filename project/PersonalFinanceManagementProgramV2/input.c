#include "input.h"

#include <stdio.h>
#include <string.h>

/*
 * input.c
 *
 * 本文件集中处理用户输入。
 *
 * 这里采用基础 C 写法：
 * 1. 整数输入使用 scanf("%d", &value)。
 * 2. 金额输入使用 scanf("%lf", &value)。
 * 3. 字符串输入使用 fgets，因为分类、备注、路径可能包含空格。
 * 4. scanf 后调用 clear_input_buffer 清理残留回车。
 * 5. read_line、read_int、read_double 等基础输入函数只在 input.c 内部使用。
 */

/*
 * 清空输入缓冲区。
 *
 * scanf 读取数字后，回车符还留在缓冲区中。
 * 为了避免后面的 fgets 或 getchar 直接读到这个回车，
 * 这里把本行剩余字符全部读掉。
 */
static void clear_input_buffer(void) {
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
        ;
    }
}

/* 去掉 fgets 读入字符串末尾的换行符 */
static void remove_newline(char *text) {
    int i;

    if (text == NULL) {
        return;
    }

    for (i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n' || text[i] == '\r') {
            text[i] = '\0';
            return;
        }
    }
}

/* 读取一行字符串 */
static void read_line(const char *prompt, char *buffer, int buffer_size) {
    if (buffer == NULL || buffer_size <= 0) {
        return;
    }

    if (prompt != NULL) {
        printf("%s", prompt);
    }

    if (fgets(buffer, buffer_size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    remove_newline(buffer);
}

/* 读取一个整数 */
static int read_int(const char *prompt) {
    int value;

    while (1) {
        if (prompt != NULL) {
            printf("%s", prompt);
        }

        if (scanf("%d", &value) == 1) {
            clear_input_buffer();
            return value;
        }

        clear_input_buffer();
        printf("输入错误，请输入整数。\n");
    }
}

/* 读取一个 double 数字 */
static double read_double(const char *prompt) {
    double value;

    while (1) {
        if (prompt != NULL) {
            printf("%s", prompt);
        }

        if (scanf("%lf", &value) == 1) {
            clear_input_buffer();
            return value;
        }

        clear_input_buffer();
        printf("输入错误，请输入数字。\n");
    }
}

/*
 * 读取时间。
 * 用户直接回车：使用当前时间。
 * 用户输入内容：按照 YYYY-MM-DD HH:MM:SS 解析。
 */
static void read_datetime_or_now(const char *prompt, DateTime *dt) {
    char line[64];

    if (dt == NULL) {
        return;
    }

    while (1) {
        read_line(prompt, line, sizeof(line));

        if (line[0] == '\0') {
            current_datetime(dt);
            return;
        }

        if (parse_datetime(line, dt)) {
            return;
        }

        printf("时间格式错误，请输入 YYYY-MM-DD HH:MM:SS，或直接回车使用当前时间。\n");
    }
}

/* 暂停屏幕，直接使用 getchar */
void input_pause(void) {
    printf("按回车继续...");
    getchar();
}

/* 输入菜单选项 */
int input_menu_choice(int max_option) {
    int choice;

    while (1) {
        choice = read_int("请选择：");

        if (choice >= 0 && choice <= max_option) {
            return choice;
        }

        printf("输入错误，请输入 0 到 %d 之间的整数。\n", max_option);
    }
}

/*
 * 输入并构建一条完整收支记录。
 *
 * 新增和修改记录都可以复用这个函数。
 * 修改记录时，程序会重新输入一整条记录，然后覆盖原记录。
 */
int input_record(Record *record) {
    int type_choice;
    double amount;

    if (record == NULL) {
        return 0;
    }

    memset(record, 0, sizeof(Record));

    while (1) {
        type_choice = read_int("类型：1.收入  2.支出：");
        if (type_choice == 1 || type_choice == 2) {
            break;
        }
        printf("类型错误，请输入 1 或 2。\n");
    }

    if (type_choice == 1) {
        record->type = REC_INCOME;
    } else {
        record->type = REC_EXPENSE;
    }

    read_line("分类：", record->category, sizeof(record->category));
    if (record->category[0] == '\0') {
        strcpy(record->category, "未分类");
    }

    while (1) {
        amount = read_double("金额：");
        if (amount >= 0.0) {
            record->amount = amount;
            break;
        }
        printf("金额不能小于 0。\n");
    }

    read_datetime_or_now("时间 YYYY-MM-DD HH:MM:SS（回车为当前时间）：", &record->time);
    read_line("备注：", record->note, sizeof(record->note));

    return 1;
}

/*
 * 输入记录序号。
 * 屏幕上显示的序号从 1 开始，但数组下标从 0 开始，
 * 所以这里返回 choice - 1。
 */
int input_record_index(int max_count) {
    int choice;

    if (max_count <= 0) {
        return -1;
    }

    while (1) {
        choice = read_int("请输入记录序号：");

        if (choice >= 1 && choice <= max_count) {
            return choice - 1;
        }

        printf("序号错误，请输入 1 到 %d 之间的整数。\n", max_count);
    }
}

/* 删除前确认，避免误删 */
int input_confirm_delete(void) {
    char line[16];

    read_line("确认删除这条记录吗？输入 y 表示确认：", line, sizeof(line));

    if (line[0] == 'y' || line[0] == 'Y') {
        return 1;
    }

    return 0;
}

/* 输入日/周/月统计条件，并转换成统计起止时间 */
int input_period_range(PeriodInput *period) {
    int level;
    char date_text[32];

    if (period == NULL) {
        return 0;
    }

    while (1) {
        level = read_int("统计级别：1.日  2.周  3.月：");
        if (level >= 1 && level <= 3) {
            break;
        }
        printf("统计级别错误，请输入 1、2 或 3。\n");
    }

    read_line("请输入日期 YYYY-MM-DD：", date_text, sizeof(date_text));

    if (!period_range_from_date(date_text, level, &period->start_time, &period->end_time)) {
        printf("日期输入错误。\n");
        return 0;
    }

    return 1;
}

/* 输入图表类型，并转换成收入/支出类型 */
RecordType input_chart_record_type(void) {
    int choice;

    while (1) {
        choice = read_int("图表类型：1.收入分类  2.支出分类：");

        if (choice == 1) {
            return REC_INCOME;
        }

        if (choice == 2) {
            return REC_EXPENSE;
        }

        printf("图表类型错误，请输入 1 或 2。\n");
    }
}

/* 输入查看记录时的排序条件 */
int input_list_option(ListInput *option) {
    int field_choice;
    int order_choice;

    if (option == NULL) {
        return 0;
    }

    while (1) {
        field_choice = read_int("排序字段：1.发生时间  2.金额：");
        if (field_choice == 1 || field_choice == 2) {
            break;
        }
        printf("排序字段错误，请输入 1 或 2。\n");
    }

    while (1) {
        order_choice = read_int("排序方式：1.升序  2.降序：");
        if (order_choice == 1 || order_choice == 2) {
            break;
        }
        printf("排序方式错误，请输入 1 或 2。\n");
    }

    option->field = (ListSortField)field_choice;
    option->order = (ListSortOrder)order_choice;
    return 1;
}

/* 输入导入文件路径 */
int input_import_path(char *path, int path_size) {
    if (path == NULL || path_size <= 0) {
        return 0;
    }

    read_line("导入CSV文件路径：", path, path_size);
    if (path[0] == '\0') {
        printf("文件路径不能为空。\n");
        return 0;
    }

    return 1;
}

/* 输入导出文件路径 */
int input_export_paths(char *csv_path, int csv_path_size, char *report_path, int report_path_size) {
    if (csv_path == NULL || report_path == NULL || csv_path_size <= 0 || report_path_size <= 0) {
        return 0;
    }

    read_line("导出记录CSV路径：", csv_path, csv_path_size);
    if (csv_path[0] == '\0') {
        printf("CSV 文件路径不能为空。\n");
        return 0;
    }

    read_line("导出报表TXT路径：", report_path, report_path_size);
    if (report_path[0] == '\0') {
        printf("报表文件路径不能为空。\n");
        return 0;
    }

    return 1;
}
