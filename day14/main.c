#include <stdio.h>

typedef struct Node
{
    int value;
    struct Node *next;
} Node;

// 插入排序
void sort1(int *arr, int num)
{
    for (int i = 1; i < num; i++)
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 冒泡排序
void sort2(int *arr, int num)
{
    for (int i = 0; i < num - 1; i++)
    {
        int swaped = 0;

        for (int j = 0; j < num - 1 - i; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
                swaped = 1;
            }
        }

        if (swaped == 0)
        {
            break;
        }
    }
}

// 选择排序
void sort3(int *arr, int num)
{
    for (int i = 0; i < num - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < num; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }
        if (minIndex != i)
        {
            swap(&arr[i], &arr[minIndex]);
        }
    }
}

// 快速排序
void sort4(int *arr, int num)
{
}

// 归并排序
void sort5(int *arr, int num)
{
}

// 堆排序
void sort6(int *arr, int num)
{
}

void arrPrint(int *arr, int num)
{
    for (int i = 0; i < num; i++)
    {
        printf("%d ", *(arr + i));
    }
}

int main(int argc, char const *argv[])
{
    /*int arr[5] = {1, 2, 3, 4, 5};
    int *p = arr;
    printf("0x%x\n", *(int *)(p + 1));*/

    int arr[7] = {9, 8, 7, 6, 5, 4, 3};
    int size = sizeof(arr) / sizeof(arr[0]);
    sort3(arr, size);
    arrPrint(arr, size);

    return 0;
}
