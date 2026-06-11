#include <stdio.h>
#include "record.h"
#include "record_list.h"
#include "stats.h"

int main()
{
    // todo 改造清单
    // 1. 加入收入分类统计
    // 2. 收入和支出记录支持时分秒，记录更详细贴近实际情况
    struct RecordList list;
    initRecordList(&list);

    int choice;

    do
    {
        printf("\n========== 个人财务管理系统 ==========\n");
        printf("1. 添加收入/支出记录\n");
        printf("2. 查看所有记录\n");
        printf("3. 按月份统计收支\n");
        printf("4. 支出分类统计\n");
        printf("5. 收入分类统计\n");
        printf("6. 余额查询\n");
        printf("0. 退出系统\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            struct Record record;
            inputRecord(&record);
            addRecord(&list, record);
            printf("添加成功！\n");
            break;
        }

        case 2:
            printAllRecords(&list);
            break;

        case 3:
            monthlyStatistics(&list);
            break;

        case 4:
            expenseCategoryStatistics(&list);
            break;

        case 5:
            // expenseCategoryStatistics(&list);
            break;

        case 6:
            showBalance(&list);
            break;

        case 0:
            printf("退出系统。\n");
            break;

        default:
            printf("输入错误，请重新选择。\n");
            break;
        }

    } while (choice != 0);

    freeRecordList(&list);

    return 0;
}