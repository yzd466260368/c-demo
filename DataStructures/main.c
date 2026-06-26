#include <stdio.h>

// 根据前序和中序输出后序
void getPostOrder(
    int pre[], int in[],
    int preL, int preR,
    int inL, int inR)
{
    if (preL > preR)
    {
        return;
    }

    // 前序遍历的第一个元素是根结点
    int root = pre[preL];

    // 在中序遍历中找到根结点
    int rootIndex = inL;
    while (rootIndex <= inR && in[rootIndex] != root)
    {
        rootIndex++;
    }

    // 如果没找到，说明输入数据有问题
    if (rootIndex > inR)
    {
        printf("输入的前序和中序不匹配\n");
        return;
    }

    // 左子树结点个数
    int leftSize = rootIndex - inL;

    // 递归处理左子树
    getPostOrder(
        pre, in,
        preL + 1,
        preL + leftSize,
        inL,
        rootIndex - 1);

    // 递归处理右子树
    getPostOrder(
        pre, in,
        preL + leftSize + 1,
        preR,
        rootIndex + 1,
        inR);

    // 后序遍历最后访问根结点
    printf("%d ", root);
}

int main()
{
    int pre[] = {1, 2, 4, 5, 3, 6};
    int in[] = {4, 2, 5, 1, 3, 6};

    int n = sizeof(pre) / sizeof(pre[0]);

    printf("后序遍历结果："); // 后序遍历结果：4 5 2 6 3 1
    getPostOrder(pre, in, 0, n - 1, 0, n - 1);

    return 0;
}

// #include <stdio.h>
// #include <stdint.h>

// void swap(int *a, int *b)
// {
//     int temp = *a;
//     *a = *b;
//     *b = temp;
// }

// int partion(int *arr, int left, int right)
// {
//     int flag = arr[right];

//     int i = left - 1;

//     for (int j = left; j < right; j++)
//     {
//         if (arr[j] <= flag)
//         {
//             i++;
//             swap(&arr[i], &arr[j]);
//         }
//     }
//     swap(&arr[i + 1], &arr[right]);

//     return i + 1;
// }

// void qucikSort(int *arr, int left, int right)
// {

//     if (left >= right) // 终止条件
//         return;
//     int index = partion(arr, left, right);

//     qucikSort(arr, left, index - 1);
//     qucikSort(arr, index + 1, right);
// }

// void print(int *arr, int n)
// {
//     for (int i = 0; i < n; i++)
//     {
//         printf("%d ", *(arr + i));
//     }
//     putchar('\n');
// }

// int main(int argc, char const *argv[])
// {

//     int arr[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

//     qucikSort(arr, 0, 9);
//     print(arr, 10);
//     return 0;
// }
