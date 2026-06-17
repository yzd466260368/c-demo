# 收支管理系统（分层菜单 + 简化输入版）

本项目是一个使用 C 语言实现的收支管理系统。

## 本版特点

1. 使用 `malloc / realloc / free` 实现动态数组。
2. 使用二进制文件保存和加载历史记录。
3. 菜单使用“主菜单分组 + 子菜单功能”的两层结构，避免所有功能一次性展示。
4. 子菜单仍使用 `MenuItem` 结构体数组，将菜单编号、菜单名称、执行函数绑定在一起。
5. `menu.c` 只负责菜单展示、分组查找和函数指针调度，具体输入集中放在 `input.c`。
6. `input.c` 使用更简单的基础输入函数：`scanf`、`fgets`、`getchar`。
7. `input_line`、`input_int`、`input_money`、`input_datetime_or_now` 不再暴露到头文件，改为 `input.c` 内部 `static` 辅助函数。
8. 每个模块有中文注释，便于课程设计说明。


## 菜单层级

程序启动后先显示主菜单：

```text
1. 记录管理
2. 统计查询
3. 数据管理
0. 退出系统
```

进入某个分类后，再显示该分类下的具体功能。

记录管理：

```text
1. 记录收入/支出
2. 查看全部记录
0. 返回上一级
```

统计查询：

```text
1. 按日/周/月统计收支
2. 支出分类统计
3. 收入分类统计
4. 余额查询
5. 收支数据图表
0. 返回上一级
```

数据管理：

```text
1. 导入记录
2. 导出记录和报表
0. 返回上一级
```

## 文件结构

```text
main.c              程序入口
model.c/.h          数据结构、动态数组、时间处理
storage.c/.h        二进制文件保存和加载
stats.c/.h          收支统计
charts.c/.h         星号图表
import_export.c/.h  CSV 导入导出、TXT 报表导出
input.c/.h          用户输入处理
menu.c/.h           菜单显示和函数指针调度
sample_import.csv   示例导入文件
Makefile            编译脚本
```

## 编译运行

```bash
make
./finance_manager
```

如果没有 `make`，也可以手动编译：

```bash
gcc -std=c99 -Wall -Wextra -pedantic -o finance_manager main.c model.c storage.c stats.c charts.c import_export.c input.c menu.c
```

## CSV 导入格式

```csv
type,category,amount,time,note
收入,工资,8500.00,2026-06-01 09:00:00,六月工资
支出,餐饮,38.50,2026-06-01 12:30:10,午饭
```

说明：为了保持基础实现，CSV 字段中不要包含英文逗号。
