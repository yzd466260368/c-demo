#include "menu.h"
#include "model.h"
#include "storage.h"

#include <stdio.h>

/*
 * 程序入口。
 * 整体运行步骤：
 * 1. 定义 RecordStore，用来保存所有收支记录。
 * 2. 调用 store_init 初始化动态数组。
 * 3. 调用 storage_load 从二进制文件读取历史数据。
 * 4. 调用 menu_run 进入菜单循环。
 * 5. 用户退出菜单后，调用 storage_save 自动保存数据。
 * 6. 调用 store_free 释放动态内存。
 */
int main(void) {
    RecordStore store;

    if (!store_init(&store)) {
        printf("内存分配失败，程序无法启动。\n");
        return 1;
    }

    if (storage_load(DATA_FILE, &store)) {
        printf("历史数据加载完成，共 %d 条记录。\n", store.count);
    } else {
        printf("历史数据加载失败，使用空数据启动。\n");
        store_clear(&store);
    }

    menu_run(&store);

    if (storage_save(DATA_FILE, &store)) {
        printf("数据已保存到 %s。\n", DATA_FILE);
    } else {
        printf("数据保存失败。\n");
    }

    store_free(&store);

    return 0;
}
