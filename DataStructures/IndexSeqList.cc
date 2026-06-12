#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
#define BLOCKSIZE 5
#define MAXBLOCK 20

typedef int KeyType;

/* 顺序表 */
typedef struct
{
    KeyType data[MAXSIZE];
    int length;
} SeqList;

/* 索引项 */
typedef struct
{
    KeyType maxKey; // 当前块最大关键字
    int start;      // 当前块起始下标
    int length;     // 当前块长度
} IndexItem;

/* 索引顺序表 */
typedef struct
{
    SeqList list;
    IndexItem index[MAXBLOCK];
    int blockCount;
} IndexSeqList;

/* 初始化 */
void InitList(IndexSeqList *isl)
{
    isl->list.length = 0;
    isl->blockCount = 0;
}

/* 建立索引表 */
void BuildIndex(IndexSeqList *isl)
{
    int i, j;

    isl->blockCount = (isl->list.length + BLOCKSIZE - 1) / BLOCKSIZE;

    for (i = 0; i < isl->blockCount; i++)
    {
        int start = i * BLOCKSIZE;
        int end = start + BLOCKSIZE;

        if (end > isl->list.length)
        {
            end = isl->list.length;
        }

        isl->index[i].start = start;
        isl->index[i].length = end - start;
        isl->index[i].maxKey = isl->list.data[start];

        for (j = start + 1; j < end; j++)
        {
            if (isl->list.data[j] > isl->index[i].maxKey)
            {
                isl->index[i].maxKey = isl->list.data[j];
            }
        }
    }
}

/* 创建索引顺序表 */
void CreateList(IndexSeqList *isl, KeyType arr[], int n)
{
    int i;

    if (n > MAXSIZE)
    {
        printf("数据过多，创建失败！\n");
        return;
    }

    for (i = 0; i < n; i++)
    {
        isl->list.data[i] = arr[i];
    }

    isl->list.length = n;
    BuildIndex(isl);
}

/* 查找元素 */
int Search(IndexSeqList *isl, KeyType key)
{
    int i, j;

    for (i = 0; i < isl->blockCount; i++)
    {
        if (key <= isl->index[i].maxKey)
        {
            int start = isl->index[i].start;
            int end = start + isl->index[i].length;

            for (j = start; j < end; j++)
            {
                if (isl->list.data[j] == key)
                {
                    return j;
                }
            }
            return -1;
        }
    }

    return -1;
}

/* 插入元素 */
int Insert(IndexSeqList *isl, KeyType key)
{
    if (isl->list.length >= MAXSIZE)
    {
        printf("顺序表已满，插入失败！\n");
        return 0;
    }

    isl->list.data[isl->list.length] = key;
    isl->list.length++;

    BuildIndex(isl);
    return 1;
}

/* 删除元素 */
int Delete(IndexSeqList *isl, KeyType key)
{
    int pos = Search(isl, key);
    int i;

    if (pos == -1)
    {
        printf("未找到元素，删除失败！\n");
        return 0;
    }

    for (i = pos; i < isl->list.length - 1; i++)
    {
        isl->list.data[i] = isl->list.data[i + 1];
    }

    isl->list.length--;
    BuildIndex(isl);

    return 1;
}

/* 输出数据表 */
void PrintList(IndexSeqList *isl)
{
    int i;

    printf("数据表：");
    for (i = 0; i < isl->list.length; i++)
    {
        printf("%d ", isl->list.data[i]);
    }
    printf("\n");
}

/* 输出索引表 */
void PrintIndex(IndexSeqList *isl)
{
    int i;

    printf("索引表：\n");
    printf("块号\t起始位置\t块长\t最大关键字\n");

    for (i = 0; i < isl->blockCount; i++)
    {
        printf("%d\t%d\t\t%d\t%d\n",
               i,
               isl->index[i].start,
               isl->index[i].length,
               isl->index[i].maxKey);
    }
}

/* 主函数测试 */
int main()
{
    IndexSeqList isl;

    KeyType arr[] = {
        3, 7, 12, 18, 20,
        25, 29, 31, 36, 40,
        45, 49, 52, 57, 60};

    int n = sizeof(arr) / sizeof(arr[0]);
    int pos;

    InitList(&isl);
    CreateList(&isl, arr, n);

    printf("初始化后的索引顺序表：\n");
    PrintList(&isl);
    PrintIndex(&isl);

    printf("\n查找元素 31：\n");
    pos = Search(&isl, 31);
    if (pos != -1)
    {
        printf("查找成功，元素 31 的位置为：%d\n", pos);
    }
    else
    {
        printf("查找失败，未找到元素 31\n");
    }

    printf("\n插入元素 65：\n");
    if (Insert(&isl, 65))
    {
        printf("插入成功！\n");
    }
    PrintList(&isl);
    PrintIndex(&isl);

    printf("\n删除元素 29：\n");
    if (Delete(&isl, 29))
    {
        printf("删除成功！\n");
    }
    PrintList(&isl);
    PrintIndex(&isl);

    return 0;
}