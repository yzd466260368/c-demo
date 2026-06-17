#ifndef INPUT_H
#define INPUT_H

/*
 * input.h
 *
 * 本模块专门处理用户输入。
 *
 * 设计原则：
 * 1. menu.c 不直接使用 scanf/fgets 处理输入。
 * 2. input.c 内部使用基础 C 输入函数 scanf、fgets、getchar。
 * 3. 对外只暴露业务级输入函数，例如输入记录、输入统计条件、输入排序方式。
 * 4. 这样菜单层只负责调度，输入层负责把用户输入转换成程序需要的数据。
 */

#include "model.h"

/* 周期统计输入结果：保存统计开始时间和结束时间 */
typedef struct {
    time_t start_time;
    time_t end_time;
} PeriodInput;

/* 查看记录时的排序字段 */
typedef enum {
    LIST_SORT_BY_TIME = 1,    /* 按发生时间排序 */
    LIST_SORT_BY_AMOUNT = 2   /* 按金额排序 */
} ListSortField;

/* 查看记录时的排序方向 */
typedef enum {
    LIST_ORDER_ASC = 1,       /* 升序 */
    LIST_ORDER_DESC = 2       /* 降序 */
} ListSortOrder;

/* 查看记录时的排序条件 */
typedef struct {
    ListSortField field;
    ListSortOrder order;
} ListInput;

/* 暂停屏幕 */
void input_pause(void);

/* 输入菜单选项 */
int input_menu_choice(int max_option);

/* 输入并构建一条完整收支记录 */
int input_record(Record *record);

/* 输入要修改或删除的记录序号，返回数组下标，从 0 开始 */
int input_record_index(int max_count);

/* 输入删除确认，返回 1 表示确认，返回 0 表示取消 */
int input_confirm_delete(void);

/* 输入日/周/月统计条件，并转换成起止时间 */
int input_period_range(PeriodInput *period);

/* 输入图表类型，并转换成收入/支出类型 */
RecordType input_chart_record_type(void);

/* 输入查看记录时的排序字段和排序方向 */
int input_list_option(ListInput *option);

/* 输入导入文件路径 */
int input_import_path(char *path, int path_size);

/* 输入导出文件路径 */
int input_export_paths(char *csv_path, int csv_path_size, char *report_path, int report_path_size);

#endif
