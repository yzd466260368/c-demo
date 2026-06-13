#include "model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 初始化动态数组。
 * 步骤：
 * 1. 判断参数是否为空，防止空指针。
 * 2. 使用 malloc 分配初始容量。
 * 3. 分配失败时把 count 和 capacity 设为 0。
 * 4. 分配成功时记录当前数量和容量。
 */
int store_init(RecordStore *store) {
    if (store == NULL) {
        return 0;
    }

    store->items = (Record *)malloc(sizeof(Record) * INIT_CAPACITY);
    if (store->items == NULL) {
        store->count = 0;
        store->capacity = 0;
        return 0;
    }

    store->count = 0;
    store->capacity = INIT_CAPACITY;
    return 1;
}

/*
 * 释放动态数组。
 * 步骤：
 * 1. 判断参数是否为空。
 * 2. free 释放 malloc/realloc 得到的堆空间。
 * 3. 把指针置为 NULL，避免野指针。
 * 4. 把数量和容量清零，表示当前没有可用数据。
 */
void store_free(RecordStore *store) {
    if (store == NULL) {
        return;
    }

    free(store->items);
    store->items = NULL;
    store->count = 0;
    store->capacity = 0;
}

/*
 * 清空记录。
 * 注意：这里只把 count 设为 0，不释放内存。
 * 这样后续继续添加记录时可以复用原来的空间，提高效率。
 */
void store_clear(RecordStore *store) {
    if (store == NULL) {
        return;
    }

    store->count = 0;
}

/*
 * 扩容函数。
 * 步骤：
 * 1. 如果新容量小于等于旧容量，说明不用扩容。
 * 2. 使用 realloc 调整堆空间大小。
 * 3. realloc 失败时返回 0，原来的 store->items 仍然有效。
 * 4. realloc 成功后更新指针和容量。
 */
int store_reserve(RecordStore *store, int new_capacity) {
    Record *new_items;

    if (store == NULL || new_capacity <= 0) {
        return 0;
    }

    if (new_capacity <= store->capacity) {
        return 1;
    }

    new_items = (Record *)realloc(store->items, sizeof(Record) * new_capacity);
    if (new_items == NULL) {
        return 0;
    }

    store->items = new_items;
    store->capacity = new_capacity;
    return 1;
}

/*
 * 添加一条记录。
 * 步骤：
 * 1. 检查 store 和 record 是否为空。
 * 2. 判断当前容量是否已满。
 * 3. 如果已满，就把容量扩大为原来的 2 倍。
 * 4. 扩容成功后，把传入的记录复制到数组末尾。
 * 5. count 加 1，表示记录数量增加。
 *
 * record 参数加 const，表示本函数只读取 record，不会修改它。
 */
int store_add(RecordStore *store, const Record *record) {
    int new_capacity;

    if (store == NULL || record == NULL) {
        return 0;
    }

    if (store->count >= store->capacity) {
        if (store->capacity <= 0) {
            new_capacity = INIT_CAPACITY;
        } else {
            new_capacity = store->capacity * 2;
        }

        if (!store_reserve(store, new_capacity)) {
            return 0;
        }
    }

    store->items[store->count] = *record;
    store->count++;
    return 1;
}

/* 把枚举类型转换成中文文本，方便输出显示 */
const char *record_type_to_text(RecordType type) {
    if (type == REC_INCOME) {
        return "收入";
    }
    if (type == REC_EXPENSE) {
        return "支出";
    }
    return "未知";
}

/*
 * 把用户输入或 CSV 中的文本转换成 RecordType。
 * 支持中文、英文和数字三种写法，方便导入数据。
 */
int parse_record_type(const char *text, RecordType *type) {
    if (text == NULL || type == NULL) {
        return 0;
    }

    if (strcmp(text, "1") == 0 || strcmp(text, "收入") == 0 || strcmp(text, "income") == 0) {
        *type = REC_INCOME;
        return 1;
    }

    if (strcmp(text, "2") == 0 || strcmp(text, "支出") == 0 || strcmp(text, "expense") == 0) {
        *type = REC_EXPENSE;
        return 1;
    }

    return 0;
}

/*
 * 解析完整日期时间，格式：YYYY-MM-DD HH:MM:SS。
 * 步骤：
 * 1. 使用 sscanf 从字符串中读取年月日时分秒。
 * 2. 检查每个字段是否在基本合法范围内。
 * 3. 合法则复制给输出参数 dt。
 */
int parse_datetime(const char *text, DateTime *dt) {
    DateTime temp;

    if (text == NULL || dt == NULL) {
        return 0;
    }

    if (sscanf(text, "%d-%d-%d %d:%d:%d",
               &temp.year, &temp.month, &temp.day,
               &temp.hour, &temp.minute, &temp.second) != 6) {
        return 0;
    }

    if (temp.year < 1900 || temp.month < 1 || temp.month > 12 ||
        temp.day < 1 || temp.day > 31 ||
        temp.hour < 0 || temp.hour > 23 ||
        temp.minute < 0 || temp.minute > 59 ||
        temp.second < 0 || temp.second > 59) {
        return 0;
    }

    *dt = temp;
    return 1;
}

/*
 * 只解析日期，格式：YYYY-MM-DD。
 * 解析成功后，把时分秒设置成 00:00:00。
 */
int parse_date(const char *text, DateTime *dt) {
    DateTime temp;

    if (text == NULL || dt == NULL) {
        return 0;
    }

    if (sscanf(text, "%d-%d-%d", &temp.year, &temp.month, &temp.day) != 3) {
        return 0;
    }

    if (temp.year < 1900 || temp.month < 1 || temp.month > 12 || temp.day < 1 || temp.day > 31) {
        return 0;
    }

    temp.hour = 0;
    temp.minute = 0;
    temp.second = 0;
    *dt = temp;
    return 1;
}

/*
 * 获取当前系统时间。
 * time(NULL) 得到时间戳，再转换成 DateTime 结构。
 */
void current_datetime(DateTime *dt) {
    time_t now;

    if (dt == NULL) {
        return;
    }

    now = time(NULL);
    time_t_to_datetime(now, dt);
}

/*
 * 把 DateTime 转换成 time_t。
 * 统计日/周/月时，time_t 比较大小更方便。
 */
time_t datetime_to_time_t(const DateTime *dt) {
    struct tm t;

    if (dt == NULL) {
        return (time_t)-1;
    }

    memset(&t, 0, sizeof(t));
    t.tm_year = dt->year - 1900;
    t.tm_mon = dt->month - 1;
    t.tm_mday = dt->day;
    t.tm_hour = dt->hour;
    t.tm_min = dt->minute;
    t.tm_sec = dt->second;
    t.tm_isdst = -1;

    return mktime(&t);
}

/*
 * 把 time_t 转换成 DateTime。
 * localtime 是 C 标准库函数，适合基础 C 课程设计使用。
 */
void time_t_to_datetime(time_t value, DateTime *dt) {
    struct tm *p;

    if (dt == NULL) {
        return;
    }

    p = localtime(&value);
    if (p == NULL) {
        memset(dt, 0, sizeof(DateTime));
        return;
    }

    dt->year = p->tm_year + 1900;
    dt->month = p->tm_mon + 1;
    dt->day = p->tm_mday;
    dt->hour = p->tm_hour;
    dt->minute = p->tm_min;
    dt->second = p->tm_sec;
}

/*
 * 把 DateTime 格式化成字符串，方便在屏幕、CSV、报表中输出。
 */
void format_datetime(const DateTime *dt, char *buffer, int buffer_size) {
    if (dt == NULL || buffer == NULL || buffer_size <= 0) {
        return;
    }

    snprintf(buffer, (unsigned int)buffer_size, "%04d-%02d-%02d %02d:%02d:%02d",
             dt->year, dt->month, dt->day,
             dt->hour, dt->minute, dt->second);
}

/*
 * 根据某一天和统计级别计算统计区间。
 * level = 1：当天 00:00:00 到下一天 00:00:00
 * level = 2：本周周一 00:00:00 到下周周一 00:00:00
 * level = 3：本月 1 日 00:00:00 到下月 1 日 00:00:00
 */
int period_range_from_date(const char *date_text, int level, time_t *start_time, time_t *end_time) {
    DateTime date;
    struct tm t;
    int monday_offset;

    if (start_time == NULL || end_time == NULL) {
        return 0;
    }

    if (!parse_date(date_text, &date)) {
        return 0;
    }

    memset(&t, 0, sizeof(t));
    t.tm_year = date.year - 1900;
    t.tm_mon = date.month - 1;
    t.tm_mday = date.day;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1;

    if (level == 1) {
        *start_time = mktime(&t);
        t.tm_mday += 1;
        *end_time = mktime(&t);
        return 1;
    }

    if (level == 2) {
        mktime(&t);
        monday_offset = (t.tm_wday + 6) % 7;
        t.tm_mday -= monday_offset;
        *start_time = mktime(&t);
        t.tm_mday += 7;
        *end_time = mktime(&t);
        return 1;
    }

    if (level == 3) {
        t.tm_mday = 1;
        *start_time = mktime(&t);
        t.tm_mon += 1;
        *end_time = mktime(&t);
        return 1;
    }

    return 0;
}
