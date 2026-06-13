# 收支管理系统（基础 C 语言版）

本项目使用基础 C 语言实现一个收支管理系统，适合课程设计或 C 语言练习使用。

## 功能说明

系统支持：

1. 新增收入 / 支出记录
2. 修改已有记录
3. 删除已有记录
4. 查看记录，并支持：
   - 按发生时间升序 / 降序
   - 按金额升序 / 降序
5. 按日 / 周 / 月统计收入和支出
6. 支出分类统计
7. 收入分类统计
8. 余额查询
9. 使用 `*` 号显示简单数据图表
10. 从 CSV 文件导入记录
11. 导出记录 CSV 和统计报表 TXT
12. 启动时从二进制文件 `records.dat` 加载历史数据
13. 退出时自动保存记录到二进制文件 `records.dat`

## 代码特点

- 使用 `malloc / realloc / free` 管理动态数组
- 使用结构体数组维护分层菜单
- 菜单项统一绑定：菜单编号、菜单名称、执行函数
- 使用函数指针完成菜单功能调用，减少 `switch-case`
- 用户输入逻辑集中在 `input.c`
- 记录增删改查逻辑由 `model.c` 提供基础操作
- 统计逻辑集中在 `stats.c`
- 文件导入导出逻辑集中在 `import_export.c`
- 二进制保存和加载逻辑集中在 `storage.c`
- 只读函数参数尽量使用 `const`

## 菜单结构

主菜单：

```text
1. 记录管理
2. 统计查询
3. 数据管理
0. 退出系统
```

记录管理：

```text
1. 新增记录
2. 修改记录
3. 删除记录
4. 查看记录
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

## 编译运行

在 Linux / macOS / MinGW 环境下，可以使用：

```bash
make
./finance_manager
```

也可以手动编译：

```bash
gcc -std=c99 -Wall -Wextra -pedantic -o finance_manager main.c model.c storage.c stats.c charts.c import_export.c input.c menu.c
```

清理编译结果：

```bash
make clean
```

## CSV 导入格式

CSV 文件格式如下：

```csv
type,category,amount,time,note
收入,工资,8500.00,2026-06-01 09:00:00,六月工资
支出,餐饮,38.50,2026-06-01 12:30:10,午饭
```

字段说明：

- `type`：收入 / 支出，也支持 `income` / `expense` 或 `1` / `2`
- `category`：分类
- `amount`：金额
- `time`：发生时间，格式为 `YYYY-MM-DD HH:MM:SS`
- `note`：备注

为了保持代码简单，CSV 只支持普通英文逗号分隔，字段中不要包含英文逗号。

## 文件说明

```text
main.c            程序入口，负责初始化、加载、菜单运行、保存、释放内存
model.h/.c        核心数据结构、动态数组、记录新增/修改/删除、时间处理
input.h/.c        用户输入处理，把输入转换成 Record、排序条件、统计条件等
menu.h/.c         分层菜单展示和函数指针调度
stats.h/.c        日/周/月统计、分类统计、余额统计
charts.h/.c       星号图表输出
import_export.h/.c CSV 导入、CSV 导出、TXT 报表导出
storage.h/.c      二进制文件保存和加载
Makefile          编译脚本
sample_import.csv 示例导入文件
```
