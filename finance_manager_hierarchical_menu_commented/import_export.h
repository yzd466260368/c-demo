#ifndef IMPORT_EXPORT_H
#define IMPORT_EXPORT_H

/*
 * import_export.h
 *
 * 本模块负责 CSV 导入、CSV 导出和 TXT 报表导出。
 */

#include "model.h"

int import_records_csv(const char *file_path, RecordStore *store, int *imported_count);
int export_records_csv(const char *file_path, const RecordStore *store);
int export_report_txt(const char *file_path, const RecordStore *store);

#endif
