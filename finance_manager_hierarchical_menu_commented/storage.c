#include "storage.h"

#include <stdio.h>
#include <string.h>

/* 文件标识，用来判断 records.dat 是否是本程序生成的数据文件 */
#define DATA_MAGIC "FINDYN"

/* 文件版本号，后续如果结构改变，可以通过版本号兼容旧数据 */
#define DATA_VERSION 1

/* 防止损坏文件中写入一个特别大的 count，导致程序尝试分配过多内存 */
#define MAX_LOAD_RECORDS 1000000

/*
 * 二进制文件头。
 * magic   ：文件标识
 * version ：文件版本
 * count   ：文件中保存了多少条记录
 */
typedef struct {
    char magic[16];
    int version;
    int count;
} FileHeader;

/*
 * 从二进制文件加载记录。
 * 步骤：
 * 1. 打开 records.dat。
 * 2. 如果文件不存在，说明第一次运行，直接使用空数据。
 * 3. 读取文件头，检查 magic、version、count 是否合法。
 * 4. 根据 count 扩容动态数组。
 * 5. 一次性读取所有 Record 数据。
 * 6. 读取成功后设置 store->count。
 */
int storage_load(const char *file_path, RecordStore *store) {
    FILE *fp;
    FileHeader header;
    int read_count;
    int need_capacity;

    if (file_path == NULL || store == NULL) {
        return 0;
    }

    fp = fopen(file_path, "rb");
    if (fp == NULL) {
        store_clear(store);
        return 1;
    }

    if (fread(&header, sizeof(FileHeader), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }

    if (strcmp(header.magic, DATA_MAGIC) != 0 ||
        header.version != DATA_VERSION ||
        header.count < 0 || header.count > MAX_LOAD_RECORDS) {
        fclose(fp);
        return 0;
    }

    if (header.count > INIT_CAPACITY) {
        need_capacity = header.count;
    } else {
        need_capacity = INIT_CAPACITY;
    }

    if (!store_reserve(store, need_capacity)) {
        fclose(fp);
        return 0;
    }

    read_count = 0;
    if (header.count > 0) {
        read_count = (int)fread(store->items, sizeof(Record), header.count, fp);
    }

    fclose(fp);

    if (read_count != header.count) {
        store_clear(store);
        return 0;
    }

    store->count = header.count;
    return 1;
}

/*
 * 把记录保存到二进制文件。
 * 步骤：
 * 1. 以 wb 方式打开文件，旧文件会被覆盖。
 * 2. 准备文件头，写入 magic、version、count。
 * 3. 再把动态数组中的 Record 一次性写入文件。
 * 4. 判断写入数量是否等于记录数量。
 *
 * store 参数使用 const，说明保存过程只读取数据，不修改数据。
 */
int storage_save(const char *file_path, const RecordStore *store) {
    FILE *fp;
    FileHeader header;
    int write_count;

    if (file_path == NULL || store == NULL) {
        return 0;
    }

    fp = fopen(file_path, "wb");
    if (fp == NULL) {
        return 0;
    }

    memset(&header, 0, sizeof(FileHeader));
    strcpy(header.magic, DATA_MAGIC);
    header.version = DATA_VERSION;
    header.count = store->count;

    if (fwrite(&header, sizeof(FileHeader), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }

    write_count = 0;
    if (store->count > 0) {
        write_count = (int)fwrite(store->items, sizeof(Record), store->count, fp);
    }

    fclose(fp);
    return write_count == store->count;
}
