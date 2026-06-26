#include <stdio.h>
#include <stdlib.h>

/* 交换两个整数 */
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* 打印数组 */
void print_array(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/* 复制数组，方便测试不同排序 */
void copy_array(int dest[], int src[], int n)
{
    for (int i = 0; i < n; i++)
    {
        dest[i] = src[i];
    }
}

/* =====================================================
 * 1. 插入排序
 *
 * 记忆：
 * 从第二个元素开始，把当前元素 key 插入到前面有序区间中。
 *
 * 核心：
 * 前面比 key 大的元素后移，最后把 key 放到空位。
 * =====================================================
 */
void insertion_sort(int arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = arr[i]; // 当前要插入的元素
        int j = i - 1;    // j 指向前面有序区间的最后一个元素

        // 只要前面的元素比 key 大，就向后移动
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        // j + 1 就是 key 应该插入的位置
        arr[j + 1] = key;
    }
}

/*
希尔排序（升级版插入排序）
*/
void shellSort(int arr[], int n)
{
    int gap, i, j, key;

    // gap 逐渐缩小
    for (gap = n / 2; gap > 0; gap /= 2)
    {
        // 对 gap 间隔的元素做插入排序
        for (i = gap; i < n; i++)
        {
            key = arr[i];
            j = i;

            while (j >= gap && arr[j - gap] > key)
            {
                arr[j] = arr[j - gap];
                j -= gap;
            }

            arr[j] = key;
        }
    }
}

/* =====================================================
 * 2. 冒泡排序
 *
 * 记忆：
 * 相邻元素两两比较，大的往后冒。
 *
 * 核心：
 * 每一轮都会把当前最大值放到最后。
 * =====================================================
 */
void bubble_sort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int swapped = 0; // 记录本轮是否发生交换

        // n - 1 - i 是为了避免 arr[j + 1] 越界
        // 同时跳过后面已经排好的元素
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }

        // 如果一轮都没有交换，说明数组已经有序
        if (swapped == 0)
        {
            break;
        }
    }
}

/* =====================================================
 * 3. 快速排序
 *
 * 记忆：
 * 选一个基准值 pivot，小的放左边，大的放右边。
 *
 * 这里使用中间元素作为 pivot，代码比较简洁。
 * =====================================================
 */
void quick_sort_core(int arr[], int left, int right)
{
    if (left >= right)
    {
        return;
    }

    int i = left;
    int j = right;
    int pivot = arr[left + (right - left) / 2]; // 选中间元素为基准

    while (i <= j)
    {
        // 从左边找第一个 >= pivot 的元素
        while (arr[i] < pivot)
        {
            i++;
        }

        // 从右边找第一个 <= pivot 的元素
        while (arr[j] > pivot)
        {
            j--;
        }

        // 如果 i 和 j 没有交错，就交换
        if (i <= j)
        {
            swap(&arr[i], &arr[j]);
            i++;
            j--;
        }
    }

    // 递归排序左半部分
    quick_sort_core(arr, left, j);

    // 递归排序右半部分
    quick_sort_core(arr, i, right);
}

void quick_sort(int arr[], int n)
{
    quick_sort_core(arr, 0, n - 1);
}

/* =====================================================
 * 4. 选择排序
 *
 * 记忆：
 * 每一轮从后面找最小值，放到当前位置。
 *
 * 核心：
 * 第 i 轮确定 arr[i] 的最终位置。
 * =====================================================
 */
void selection_sort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int min_index = i; // 假设当前位置就是最小值位置

        // 从 i 后面找真正的最小值
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[min_index])
            {
                min_index = j;
            }
        }

        // 把最小值交换到当前位置
        if (min_index != i)
        {
            swap(&arr[i], &arr[min_index]);
        }
    }
}

/* =====================================================
 * 5. 归并排序
 *
 * 记忆：
 * 先拆分，再合并。
 *
 * 核心：
 * 把两个有序数组合并成一个有序数组。
 * =====================================================
 */
void merge(int arr[], int temp[], int left, int mid, int right)
{
    int i = left;    // 左半部分起点
    int j = mid + 1; // 右半部分起点
    int k = left;    // 临时数组下标

    // 比较左右两边，小的先放入 temp
    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
        {
            temp[k++] = arr[i++];
        }
        else
        {
            temp[k++] = arr[j++];
        }
    }

    // 左边如果还有剩余，直接放入 temp
    while (i <= mid)
    {
        temp[k++] = arr[i++];
    }

    // 右边如果还有剩余，直接放入 temp
    while (j <= right)
    {
        temp[k++] = arr[j++];
    }

    // 把 temp 中排好的数据复制回 arr
    for (int p = left; p <= right; p++)
    {
        arr[p] = temp[p];
    }
}

void merge_sort_core(int arr[], int temp[], int left, int right)
{
    if (left >= right)
    {
        return;
    }

    int mid = left + (right - left) / 2;

    // 排左半部分
    merge_sort_core(arr, temp, left, mid);

    // 排右半部分
    merge_sort_core(arr, temp, mid + 1, right);

    // 合并两个有序部分
    merge(arr, temp, left, mid, right);
}

void merge_sort(int arr[], int n)
{
    int *temp = (int *)malloc(sizeof(int) * n);

    if (temp == NULL)
    {
        printf("内存分配失败\n");
        return;
    }

    merge_sort_core(arr, temp, 0, n - 1);

    free(temp);
}

/* =====================================================
 * 6. 堆排序
 *
 * 记忆：
 * 先建最大堆，再不断把堆顶最大值放到数组末尾。
 *
 * 最大堆：
 * 父节点 >= 左孩子
 * 父节点 >= 右孩子
 * =====================================================
 */
void heapify(int arr[], int n, int root)
{
    int largest = root;       // 假设 root 最大
    int left = root * 2 + 1;  // 左孩子下标
    int right = root * 2 + 2; // 右孩子下标

    // 如果左孩子存在，并且更大
    if (left < n && arr[left] > arr[largest])
    {
        largest = left;
    }

    // 如果右孩子存在，并且更大
    if (right < n && arr[right] > arr[largest])
    {
        largest = right;
    }

    // 如果最大值不是 root，交换后继续调整
    if (largest != root)
    {
        swap(&arr[root], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heap_sort(int arr[], int n)
{
    // 第一步：建最大堆
    // 最后一个非叶子节点是 n / 2 - 1
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }

    // 第二步：把最大值一个个放到数组末尾
    for (int i = n - 1; i > 0; i--)
    {
        // arr[0] 是当前最大值
        swap(&arr[0], &arr[i]);

        // 重新调整剩余部分为最大堆
        heapify(arr, i, 0);
    }
}

/* =====================================================
 * 7. 树形排序
 *
 * 记忆：
 * 建二叉搜索树，然后中序遍历。
 *
 * 二叉搜索树：
 * 左子树 < 根节点 < 右子树
 *
 * 中序遍历：
 * 左 -> 根 -> 右
 *
 * 所以中序遍历结果就是升序。
 * =====================================================
 */
typedef struct TreeNode
{
    int value;              // 节点的值
    int count;              // 处理重复元素
    struct TreeNode *left;  // 左孩子
    struct TreeNode *right; // 右孩子
} TreeNode;

/* 创建新节点 */
TreeNode *create_node(int value)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));

    if (node == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }

    node->value = value;
    node->count = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/* 插入节点 */
TreeNode *insert_node(TreeNode *root, int value)
{
    if (root == NULL)
    {
        return create_node(value);
    }

    if (value < root->value)
    {
        root->left = insert_node(root->left, value);
    }
    else if (value > root->value)
    {
        root->right = insert_node(root->right, value);
    }
    else
    {
        // 如果值相等，说明出现重复元素
        root->count++;
    }

    return root;
}

/* 中序遍历，把树中的数据写回数组 */
void inorder(TreeNode *root, int arr[], int *index)
{
    if (root == NULL)
    {
        return;
    }

    inorder(root->left, arr, index);

    // 重复的数字要写入 count 次
    for (int i = 0; i < root->count; i++)
    {
        arr[*index] = root->value;
        (*index)++;
    }

    inorder(root->right, arr, index);
}

/* 释放二叉树 */
void free_tree(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void tree_sort(int arr[], int n)
{
    TreeNode *root = NULL;

    // 先把数组元素插入二叉搜索树
    for (int i = 0; i < n; i++)
    {
        root = insert_node(root, arr[i]);
    }

    // 再通过中序遍历写回数组
    int index = 0;
    inorder(root, arr, &index);

    // 释放树
    free_tree(root);
}

/* =====================================================
 * 测试代码
 * =====================================================
 */
int main(void)
{
    int original[] = {64, 34, 25, 12, 22, 11, 90, 34, 5};
    int n = sizeof(original) / sizeof(original[0]);

    int arr[100];

    printf("原始数组：\n");
    print_array(original, n);

    copy_array(arr, original, n);
    insertion_sort(arr, n);
    printf("插入排序：\n");
    print_array(arr, n);

    copy_array(arr, original, n);
    bubble_sort(arr, n);
    printf("冒泡排序：\n");
    print_array(arr, n);

    copy_array(arr, original, n);
    quick_sort(arr, n);
    printf("快速排序：\n");
    print_array(arr, n);

    copy_array(arr, original, n);
    selection_sort(arr, n);
    printf("选择排序：\n");
    print_array(arr, n);

    copy_array(arr, original, n);
    merge_sort(arr, n);
    printf("归并排序：\n");
    print_array(arr, n);

    copy_array(arr, original, n);
    heap_sort(arr, n);
    printf("堆排序：\n");
    print_array(arr, n);

    copy_array(arr, original, n);
    tree_sort(arr, n);
    printf("树形排序：\n");
    print_array(arr, n);

    return 0;
}