# 编译器
CC = gcc

# 编译参数：
# -std=c99       使用 C99 标准
# -Wall -Wextra  打开常见警告
# -pedantic      尽量检查是否符合标准 C
CFLAGS = -std=c99 -Wall -Wextra -pedantic

# 生成的可执行文件名
TARGET = finance_manager

# 需要参与编译的目标文件
OBJS = main.o model.o storage.o stats.o charts.o import_export.o input.o menu.o

# 默认目标：把所有 .o 文件链接成最终程序
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 通用规则：每个 .c 文件编译成对应的 .o 文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理编译结果和运行生成的文件
clean:
	rm -f $(OBJS) $(TARGET) records.dat records.csv report.txt

.PHONY: clean
