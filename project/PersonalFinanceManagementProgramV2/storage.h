#ifndef STORAGE_H
#define STORAGE_H

/*
 * storage.h
 *
 * 本模块负责二进制文件读写。
 * main.c 只需要调用 storage_load 和 storage_save，
 * 不需要关心文件内部如何保存。
 */

#include "model.h"

/* 从二进制文件加载历史记录 */
int storage_load(const char *file_path, RecordStore *store);

/* 把当前记录保存到二进制文件 */
int storage_save(const char *file_path, const RecordStore *store);

#endif
