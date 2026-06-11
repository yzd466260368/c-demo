#include <stdio.h>
#include <stdlib.h>

#define DataType int

typedef struct SeqList
{
    DataType *data;
    size_t size;
    size_t index;
} Seq;

// void InitSeqList(Seq **seq, size_t len)
// {
//     *seq = calloc(1, sizeof(Seq));
//     if (*seq == NULL)
//         return; // 分配失败

//     (*seq)->data = calloc(len, sizeof(DataType));
//     if ((*seq)->data == NULL)
//     {
//         free(*seq);
//         *seq = NULL;
//         return; // 分配失败
//     }

//     (*seq)->size = len;
//     (*seq)->index = 0;
// }

// 1、 初始化
Seq *InitSeqList(size_t len)
{
    // 分配结构体内存空间
    Seq *seq = (Seq *)calloc(1, sizeof(Seq));
    if (seq == NULL) // 理论上来说小于4096的申请都会成功
    {
        return NULL;
    }
    // 分配数据空间
    seq->data = (DataType *)calloc(len, sizeof(DataType));
    if (seq->data == NULL)
    {
        // 如果内存分配失败，恢复
        free(seq);
        seq = NULL;
        return NULL;
    }
    seq->index = 0;
    seq->size = len;
    return seq;
}

// 2、 释放顺序表
void ReleaseSeqList(Seq *seq)
{
    if (seq == NULL)
    {
        return;
    }
    free(seq->data);
    free(seq);
}

// 3、判断顺序表是否满
int IsSeqListFull(Seq *seq)
{
    if (seq == NULL)
    {
        return -1;
    }
    return seq->index == seq->size;
}

// 4、判断顺序表是否为空
int IsSeqListEmpty(Seq *seq)
{
    if (seq == NULL)
    {
        return 0;
    }
    return seq->index == 0;
}

// 5、向指定位置插入一个元素，返回0表示成功，-1是错误
int InsertSeqList(Seq *seq, size_t pos, DataType value)
{
    if (IsSeqListFull(seq))
    {
        return -1;
    }
    if (pos >= seq->index)
    {
        pos = seq->index;
    }

    for (int i = seq->size; i > pos; i--)
    {
        seq->data[i] = seq->data[i - 1];
    }
    seq->data[pos] = value;
    seq->index++;
    return 0;
}

// 6、尾插法
int PushBack(Seq *seq, DataType value)
{
    if (IsSeqListFull(seq))
    {
        return -1;
    }
    seq->data[seq->index++] = value;
    return 0;
}

// 7、尾删法
int PopBack(Seq *seq)
{
    if (IsSeqListFull(seq))
    {
        return -1;
    }
    seq->index--;
    return 0;
}

// 8、清空顺序表
int ClearSeqList(Seq *seq)
{
    if (IsSeqListFull(seq))
    {
        return -1;
    }
    seq->index = 0;
    return 0;
}

// 9、删除指定数据
int RemoveSeqList(Seq *seq, DataType value)
{
    if (IsSeqListEmpty(seq))
    {
        return -1;
    }
    int i = 0;
    for (int j = 0; j < seq->index; j++)
    {
        if (seq->data[j] != value)
        {
            seq->data[i++] = seq->data[j];
        }
    }
    int removeNum = seq->index - i;
    seq->index = i;
    return removeNum;
}

void Resize(Seq *seq, size_t new_size)
{
    if (seq == NULL)
        return; // 空指针保护
    if (new_size <= seq->size)
        return; // 新容量不小于当前容量时无需操作
    // 使用 realloc 扩容
    DataType *new_data = realloc(seq->data, new_size * sizeof(DataType));
    if (new_data == NULL)
    {
        // 分配失败，保持原数组不变
        fprintf(stderr, "Resize failed: not enough memory\n");
        return;
    }

    // 如果容量增加，需要将新空间初始化为 0（可选）
    for (size_t i = seq->size; i < new_size; i++)
    {
        new_data[i] = 0; // DataType 为 int 时这样初始化
    }

    seq->data = new_data;
    seq->size = new_size;
}

// 展示
void Show(Seq *seq)
{
    for (int i = 0; i < seq->index; i++)
    {
        printf("%d ", seq->data[i]);
    }
    putchar('\n');
}

int main(int argc, char const *argv[])
{
    Seq *seq = InitSeqList(100);

    InsertSeqList(seq, 0, 10);
    InsertSeqList(seq, 0, 20);
    InsertSeqList(seq, 0, 30);
    InsertSeqList(seq, 0, 40);
    InsertSeqList(seq, 0, 50);
    PushBack(seq, 100);
    PushBack(seq, 200);
    Show(seq);
    ReleaseSeqList(seq);

    return 0;
}
