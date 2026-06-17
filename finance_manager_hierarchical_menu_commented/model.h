#ifndef MODEL_H
#define MODEL_H

/*
 * model.h
 *
 * 本文件负责定义系统中最核心的数据结构和基础函数声明。
 * 其他模块都可以包含这个头文件，但 model.h 不依赖其他业务模块，
 * 这样可以减少文件之间的相互依赖。
 */

#include <time.h>

/* 初始动态数组容量：程序启动时先分配 16 条记录的空间 */
#define INIT_CAPACITY 16

/* 字符数组长度统一用宏管理，后续要修改长度时只改这里即可 */
#define CATEGORY_LEN 32
#define NOTE_LEN 128
#define DATETIME_LEN 32

/* 系统默认二进制数据文件名 */
#define DATA_FILE "records.dat"

/*
 * 收支类型。
 * 使用枚举可以让代码比直接写 1、2 更清晰。
 */
typedef enum {
    REC_INCOME = 1,  /* 收入 */
    REC_EXPENSE = 2  /* 支出 */
} RecordType;

/*
 * 自定义时间结构。
 * 这里不用直接把 struct tm 暴露给其他模块，
 * 是为了让系统时间格式更直观，也方便输入输出。
 */
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} DateTime;

/*
 * 一条收支记录。
 * type     ：收入或支出
 * category ：分类，例如 工资、餐饮、交通
 * amount   ：金额
 * time     ：发生时间，精确到秒
 * note     ：备注
 */
typedef struct {
    RecordType type;
    char category[CATEGORY_LEN];
    double amount;
    DateTime time;
    char note[NOTE_LEN];
} Record;

/*
 * 动态数组结构。
 * items    ：指向堆内存，保存多条 Record
 * count    ：当前已经使用的记录数量
 * capacity ：当前已经分配的容量
 */
typedef struct {
    Record *items;
    int count;
    int capacity;
} RecordStore;

/* 动态数组相关函数 */
int store_init(RecordStore *store);
void store_free(RecordStore *store);
void store_clear(RecordStore *store);
int store_reserve(RecordStore *store, int new_capacity);
int store_add(RecordStore *store, const Record *record);

/* 类型和时间转换相关函数 */
const char *record_type_to_text(RecordType type);
int parse_record_type(const char *text, RecordType *type);
int parse_datetime(const char *text, DateTime *dt);
int parse_date(const char *text, DateTime *dt);
void current_datetime(DateTime *dt);
time_t datetime_to_time_t(const DateTime *dt);
void time_t_to_datetime(time_t value, DateTime *dt);
void format_datetime(const DateTime *dt, char *buffer, int buffer_size);

/* 根据输入日期得到日、周、月的起止时间 */
int period_range_from_date(const char *date_text, int level, time_t *start_time, time_t *end_time);

#endif
