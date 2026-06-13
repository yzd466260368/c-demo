#include "import_export.h"

#include "stats.h"

#include <stdio.h>
#include <string.h>

/* 一行 CSV 的最大长度 */
#define LINE_LEN 512

/* 导入 CSV 固定 5 个字段：type, category, amount, time, note */
#define FIELD_COUNT 5

/*
 * 去掉字符串首尾空白字符。
 * 步骤：
 * 1. start 从前往后跳过空格、制表符、换行符。
 * 2. end 从后往前跳过空白字符。
 * 3. 把中间有效内容移动到字符串开头。
 */
static void trim(char *s) {
    int start = 0;
    int end;
    int i;

    if (s == NULL) {
        return;
    }

    while (s[start] == ' ' || s[start] == '\t' || s[start] == '\n' || s[start] == '\r') {
        start++;
    }

    end = (int)strlen(s) - 1;
    while (end >= start && (s[end] == ' ' || s[end] == '\t' || s[end] == '\n' || s[end] == '\r')) {
        end--;
    }

    i = 0;
    while (start <= end) {
        s[i] = s[start];
        i++;
        start++;
    }
    s[i] = '\0';
}

/*
 * 拆分一行简单 CSV。
 * 说明：
 * 为了保持基础 C 实现，这里只支持普通逗号分隔，
 * 字段中不要包含英文逗号，也不处理双引号转义。
 *
 * 步骤：
 * 1. 先把 fields 数组清空。
 * 2. 逐个字符扫描 line。
 * 3. 遇到逗号就结束当前字段，开始下一个字段。
 * 4. 遇到换行符就结束读取。
 * 5. 返回实际读取到的字段数量。
 */
static int split_csv_line(char *line, char fields[FIELD_COUNT][NOTE_LEN]) {
    int field_index = 0;
    int char_index = 0;
    int i;

    if (line == NULL) {
        return 0;
    }

    for (i = 0; i < FIELD_COUNT; i++) {
        fields[i][0] = '\0';
    }

    for (i = 0; line[i] != '\0'; i++) {
        if (line[i] == '\n' || line[i] == '\r') {
            break;
        }

        if (line[i] == ',') {
            fields[field_index][char_index] = '\0';
            trim(fields[field_index]);
            field_index++;
            char_index = 0;

            if (field_index >= FIELD_COUNT) {
                return 0;
            }
        } else {
            if (char_index < NOTE_LEN - 1) {
                fields[field_index][char_index] = line[i];
                char_index++;
            }
        }
    }

    fields[field_index][char_index] = '\0';
    trim(fields[field_index]);
    return field_index + 1;
}

/*
 * 导出 CSV 时，如果文本中包含逗号，就替换为空格。
 * 这样可以避免导出的 CSV 再导入时字段错位。
 */
static void write_text_no_comma(FILE *fp, const char *text) {
    int i;

    if (fp == NULL || text == NULL) {
        return;
    }

    for (i = 0; text[i] != '\0'; i++) {
        if (text[i] == ',') {
            fputc(' ', fp);
        } else {
            fputc(text[i], fp);
        }
    }
}

/*
 * 从 CSV 文件导入记录。
 * 步骤：
 * 1. 打开 CSV 文件。
 * 2. 一行一行读取。
 * 3. 第一行如果是表头，则跳过。
 * 4. 拆分字段。
 * 5. 解析类型、金额、时间。
 * 6. 复制分类和备注。
 * 7. 调用 store_add 加入动态数组。
 * 8. 统计成功导入的记录数。
 */
int import_records_csv(const char *file_path, RecordStore *store, int *imported_count) {
    FILE *fp;
    char line[LINE_LEN];
    char fields[FIELD_COUNT][NOTE_LEN];
    Record r;
    int line_no = 0;
    int ok_count = 0;

    if (file_path == NULL || store == NULL) {
        return 0;
    }

    fp = fopen(file_path, "r");
    if (fp == NULL) {
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        line_no++;

        if (line_no == 1 && strstr(line, "type") != NULL) {
            continue;
        }

        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') {
            continue;
        }

        if (split_csv_line(line, fields) != FIELD_COUNT) {
            printf("第 %d 行格式错误，已跳过。\n", line_no);
            continue;
        }

        if (!parse_record_type(fields[0], &r.type)) {
            printf("第 %d 行类型错误，已跳过。\n", line_no);
            continue;
        }

        if (sscanf(fields[2], "%lf", &r.amount) != 1 || r.amount < 0.0) {
            printf("第 %d 行金额错误，已跳过。\n", line_no);
            continue;
        }

        if (!parse_datetime(fields[3], &r.time)) {
            printf("第 %d 行时间错误，已跳过。\n", line_no);
            continue;
        }

        if (fields[1][0] == '\0') {
            strcpy(r.category, "未分类");
        } else {
            strncpy(r.category, fields[1], CATEGORY_LEN - 1);
            r.category[CATEGORY_LEN - 1] = '\0';
        }

        strncpy(r.note, fields[4], NOTE_LEN - 1);
        r.note[NOTE_LEN - 1] = '\0';

        if (!store_add(store, &r)) {
            printf("内存不足，停止导入。\n");
            break;
        }

        ok_count++;
    }

    fclose(fp);

    if (imported_count != NULL) {
        *imported_count = ok_count;
    }

    return 1;
}

/*
 * 导出全部记录到 CSV。
 * 步骤：
 * 1. 打开输出文件。
 * 2. 写入表头。
 * 3. 遍历每条记录。
 * 4. 把时间格式化成字符串。
 * 5. 按 CSV 字段顺序写入。
 */
int export_records_csv(const char *file_path, const RecordStore *store) {
    FILE *fp;
    int i;
    char time_text[DATETIME_LEN];
    const Record *r;

    if (file_path == NULL || store == NULL) {
        return 0;
    }

    fp = fopen(file_path, "w");
    if (fp == NULL) {
        return 0;
    }

    fprintf(fp, "type,category,amount,time,note\n");
    for (i = 0; i < store->count; i++) {
        r = &store->items[i];
        format_datetime(&r->time, time_text, sizeof(time_text));

        write_text_no_comma(fp, record_type_to_text(r->type));
        fputc(',', fp);
        write_text_no_comma(fp, r->category);
        fprintf(fp, ",%.2f,", r->amount);
        write_text_no_comma(fp, time_text);
        fputc(',', fp);
        write_text_no_comma(fp, r->note);
        fputc('\n', fp);
    }

    fclose(fp);
    return 1;
}

/*
 * 导出 TXT 报表。
 * 步骤：
 * 1. 生成当前时间。
 * 2. 统计收入分类和支出分类。
 * 3. 写入总记录数、收入总额、支出总额、余额。
 * 4. 写入收入分类明细。
 * 5. 写入支出分类明细。
 */
int export_report_txt(const char *file_path, const RecordStore *store) {
    FILE *fp;
    CategoryStatsList income_stats;
    CategoryStatsList expense_stats;
    DateTime now;
    char now_text[DATETIME_LEN];
    int i;
    double percent;

    if (file_path == NULL || store == NULL) {
        return 0;
    }

    fp = fopen(file_path, "w");
    if (fp == NULL) {
        return 0;
    }

    current_datetime(&now);
    format_datetime(&now, now_text, sizeof(now_text));
    stats_category(store, REC_INCOME, &income_stats);
    stats_category(store, REC_EXPENSE, &expense_stats);

    fprintf(fp, "收支管理系统报表\n");
    fprintf(fp, "生成时间：%s\n", now_text);
    fprintf(fp, "记录总数：%d\n", store->count);
    fprintf(fp, "收入总额：%.2f\n", stats_total_income(store));
    fprintf(fp, "支出总额：%.2f\n", stats_total_expense(store));
    fprintf(fp, "当前余额：%.2f\n\n", stats_balance(store));

    fprintf(fp, "收入分类统计\n");
    fprintf(fp, "分类,金额,笔数,占比\n");
    for (i = 0; i < income_stats.count; i++) {
        percent = 0.0;
        if (income_stats.total > 0.0) {
            percent = income_stats.items[i].amount * 100.0 / income_stats.total;
        }

        fprintf(fp, "%s,%.2f,%d,%.2f%%\n",
                income_stats.items[i].category,
                income_stats.items[i].amount,
                income_stats.items[i].count,
                percent);
    }

    fprintf(fp, "\n支出分类统计\n");
    fprintf(fp, "分类,金额,笔数,占比\n");
    for (i = 0; i < expense_stats.count; i++) {
        percent = 0.0;
        if (expense_stats.total > 0.0) {
            percent = expense_stats.items[i].amount * 100.0 / expense_stats.total;
        }

        fprintf(fp, "%s,%.2f,%d,%.2f%%\n",
                expense_stats.items[i].category,
                expense_stats.items[i].amount,
                expense_stats.items[i].count,
                percent);
    }

    fclose(fp);
    return 1;
}
