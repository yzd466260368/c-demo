#ifndef INPUT_H
#define INPUT_H

/*
 * input.h
 *
 * 本模块专门处理用户输入。
 *
 * 简化后的设计：
 * 1. 不再把 input_line、input_int、input_money、input_datetime_or_now
 *    这些基础输入函数暴露给其他模块。
 * 2. 基础输入函数只在 input.c 内部使用，外部模块看不到，也不需要关心。
 * 3. menu.c 只调用“业务输入函数”，例如 input_record、input_period_range。
 * 4. 这样 input.h 更干净，模块职责也更清楚。
 */

#include "model.h"

/* 周期统计输入结果：保存统计开始时间和结束时间 */
typedef struct {
    time_t start_time;
    time_t end_time;
} PeriodInput;

/* 暂停屏幕 */
void input_pause(void);

/* 输入主菜单选项 */
int input_menu_choice(int max_option);

/* 输入并构建一条完整收支记录 */
int input_record(Record *record);

/* 输入日/周/月统计条件，并转换成起止时间 */
int input_period_range(PeriodInput *period);

/* 输入图表类型，并转换成收入/支出类型 */
RecordType input_chart_record_type(void);

/* 输入导入文件路径 */
int input_import_path(char *path, int path_size);

/* 输入导出文件路径 */
int input_export_paths(char *csv_path, int csv_path_size, char *report_path, int report_path_size);

#endif
