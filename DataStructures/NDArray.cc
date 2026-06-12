#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/*
    自定义 N 维数组结构体

    例如：
    一维：dims = {5}
    二维：dims = {3, 4}
    三维：dims = {2, 3, 4}

    底层统一用一维数组 data 保存所有数据。
*/
typedef struct
{
    int *data;       // 真正保存数据的一维数组
    int ndim;        // 维度数量，例如二维数组 ndim = 2，三维数组 ndim = 3
    size_t *dims;    // 每一维的长度，例如 3 x 4，则 dims = {3, 4}
    size_t *strides; // 每一维的跨度，用于把多维下标转换成一维下标
    size_t total;    // 总元素个数，例如 3 x 4 = 12
} NDArray;

/*
    函数声明
*/
NDArray *ndarray_create(int ndim, const size_t dims[]);
void ndarray_free(NDArray *arr);

int ndarray_offset(const NDArray *arr, const size_t indices[], size_t *out_offset);
int ndarray_set(NDArray *arr, const size_t indices[], int value);
int ndarray_get(const NDArray *arr, const size_t indices[], int *out_value);

void ndarray_fill(NDArray *arr, int value);
void ndarray_print_info(const NDArray *arr);
void ndarray_print_2d(const NDArray *arr);

int ndarray_reshape(NDArray *arr, int new_ndim, const size_t new_dims[]);

/*
    创建一个 N 维数组

    参数：
        ndim: 维度数量
        dims: 每一维的大小

    例子：
        size_t dims[2] = {3, 4};
        NDArray *arr = ndarray_create(2, dims);

    表示创建一个 3 行 4 列的二维数组。
*/
NDArray *ndarray_create(int ndim, const size_t dims[])
{
    // 维度数量必须大于 0，dims 不能是 NULL
    if (ndim <= 0 || dims == NULL)
    {
        return NULL;
    }

    // 给 NDArray 结构体本身分配内存
    NDArray *arr = malloc(sizeof(NDArray));
    if (arr == NULL)
    {
        return NULL;
    }

    // 初始化结构体成员，避免后续出错时释放野指针
    arr->data = NULL;
    arr->dims = NULL;
    arr->strides = NULL;
    arr->ndim = ndim;
    arr->total = 1;

    // 给 dims 数组分配空间，用来保存每一维的大小
    arr->dims = malloc(sizeof(size_t) * ndim);
    if (arr->dims == NULL)
    {
        free(arr);
        return NULL;
    }

    // 给 strides 数组分配空间，用来保存每一维的跨度
    arr->strides = malloc(sizeof(size_t) * ndim);
    if (arr->strides == NULL)
    {
        free(arr->dims);
        free(arr);
        return NULL;
    }

    /*
        复制 dims，并计算总元素数量 total

        例如 dims = {3, 4}
        total = 3 * 4 = 12

        例如 dims = {2, 3, 4}
        total = 2 * 3 * 4 = 24
    */
    for (int i = 0; i < ndim; i++)
    {
        // 每一维的长度不能为 0
        if (dims[i] == 0)
        {
            free(arr->strides);
            free(arr->dims);
            free(arr);
            return NULL;
        }

        /*
            防止 size_t 乘法溢出

            如果 total * dims[i] 超过 size_t 最大值，
            就说明数组太大，不能创建。
        */
        if (arr->total > SIZE_MAX / dims[i])
        {
            free(arr->strides);
            free(arr->dims);
            free(arr);
            return NULL;
        }

        // 保存当前维度的长度
        arr->dims[i] = dims[i];

        // 更新总元素数量
        arr->total *= dims[i];
    }

    /*
        计算 strides

        strides 的作用是把多维下标转换为一维下标。

        二维数组：
            dims = {3, 4}
            strides = {4, 1}

            arr[i][j] 对应：
            index = i * 4 + j * 1

        三维数组：
            dims = {2, 3, 4}
            strides = {12, 4, 1}

            arr[z][x][y] 对应：
            index = z * 12 + x * 4 + y * 1
    */

    // 最后一维的跨度永远是 1
    arr->strides[ndim - 1] = 1;

    // 从倒数第二维开始，向前计算每一维的跨度
    for (int i = ndim - 2; i >= 0; i--)
    {
        arr->strides[i] = arr->strides[i + 1] * arr->dims[i + 1];
    }

    /*
        分配真正保存数据的一维数组

        需要的字节数是：
            total * sizeof(int)
    */
    if (arr->total > SIZE_MAX / sizeof(int))
    {
        free(arr->strides);
        free(arr->dims);
        free(arr);
        return NULL;
    }

    /*
        calloc 会把内存初始化为 0

        例如创建 3 x 4 数组后，所有元素初始值都是 0。
    */
    arr->data = calloc(arr->total, sizeof(int));
    if (arr->data == NULL)
    {
        free(arr->strides);
        free(arr->dims);
        free(arr);
        return NULL;
    }

    // 创建成功，返回数组指针
    return arr;
}

/*
    释放 N 维数组

    注意：
        只要用了 malloc / calloc，就要配套 free。
*/
void ndarray_free(NDArray *arr)
{
    // 如果传入的是 NULL，直接返回
    if (arr == NULL)
    {
        return;
    }

    // 释放真正保存数据的一维数组
    free(arr->data);

    // 释放维度数组
    free(arr->dims);

    // 释放跨度数组
    free(arr->strides);

    // 最后释放结构体本身
    free(arr);
}

/*
    根据多维下标计算一维下标

    参数：
        arr: N 维数组
        indices: 多维下标数组
        out_offset: 用来保存计算出来的一维下标

    例子：
        dims = {3, 4}
        strides = {4, 1}
        indices = {2, 3}

        offset = 2 * 4 + 3 * 1 = 11
*/
int ndarray_offset(const NDArray *arr, const size_t indices[], size_t *out_offset)
{
    // 参数合法性检查
    if (arr == NULL || indices == NULL || out_offset == NULL)
    {
        return 0;
    }

    // offset 用来累加最终的一维下标
    size_t offset = 0;

    // 遍历每一维
    for (int i = 0; i < arr->ndim; i++)
    {
        /*
            下标越界检查

            例如某一维长度是 4，
            那么合法下标只能是 0、1、2、3。
        */
        if (indices[i] >= arr->dims[i])
        {
            return 0;
        }

        /*
            累加当前维度贡献的偏移量

            通用公式：
                offset += indices[i] * strides[i]
        */
        offset += indices[i] * arr->strides[i];
    }

    // 把计算结果写入 out_offset
    *out_offset = offset;

    // 返回 1 表示计算成功
    return 1;
}

/*
    设置 N 维数组中某个位置的值

    例子：
        ndarray_set(arr, (size_t[]){1, 2}, 99);

    表示：
        arr[1][2] = 99;
*/
int ndarray_set(NDArray *arr, const size_t indices[], int value)
{
    // 数组不能为空
    if (arr == NULL)
    {
        return 0;
    }

    // 保存换算出来的一维下标
    size_t offset;

    // 先把多维下标转换成一维下标
    if (!ndarray_offset(arr, indices, &offset))
    {
        return 0;
    }

    // 在一维数组对应位置写入值
    arr->data[offset] = value;

    // 返回 1 表示设置成功
    return 1;
}

/*
    获取 N 维数组中某个位置的值

    例子：
        int value;
        ndarray_get(arr, (size_t[]){1, 2}, &value);

    表示：
        value = arr[1][2];
*/
int ndarray_get(const NDArray *arr, const size_t indices[], int *out_value)
{
    // 参数检查
    if (arr == NULL || out_value == NULL)
    {
        return 0;
    }

    // 保存换算出来的一维下标
    size_t offset;

    // 先把多维下标转换成一维下标
    if (!ndarray_offset(arr, indices, &offset))
    {
        return 0;
    }

    // 读取一维数组对应位置的值
    *out_value = arr->data[offset];

    // 返回 1 表示读取成功
    return 1;
}

/*
    把数组中的所有元素填充成同一个值

    例如：
        ndarray_fill(arr, 7);

    表示把所有元素都设置为 7。
*/
void ndarray_fill(NDArray *arr, int value)
{
    // 数组不能为空
    if (arr == NULL)
    {
        return;
    }

    // 遍历底层一维数组，逐个赋值
    for (size_t i = 0; i < arr->total; i++)
    {
        arr->data[i] = value;
    }
}

/*
    打印数组的基本信息

    包括：
        维度数量
        总元素个数
        每一维长度
        每一维跨度
*/
void ndarray_print_info(const NDArray *arr)
{
    // 数组不能为空
    if (arr == NULL)
    {
        return;
    }

    // 打印维度数量
    printf("ndim: %d\n", arr->ndim);

    // 打印总元素个数
    printf("total: %zu\n", arr->total);

    // 打印每一维的长度
    printf("dims: ");
    for (int i = 0; i < arr->ndim; i++)
    {
        printf("%zu ", arr->dims[i]);
    }
    printf("\n");

    // 打印每一维的跨度
    printf("strides: ");
    for (int i = 0; i < arr->ndim; i++)
    {
        printf("%zu ", arr->strides[i]);
    }
    printf("\n");
}

/*
    按二维矩阵形式打印数组

    注意：
        这个函数只适用于二维数组。
*/
void ndarray_print_2d(const NDArray *arr)
{
    // 如果数组为空，或者不是二维数组，就不能按矩阵打印
    if (arr == NULL || arr->ndim != 2)
    {
        printf("不是二维数组，无法按矩阵打印\n");
        return;
    }

    // 第 0 维表示行数
    size_t rows = arr->dims[0];

    // 第 1 维表示列数
    size_t cols = arr->dims[1];

    // 遍历每一行
    for (size_t i = 0; i < rows; i++)
    {
        // 遍历每一列
        for (size_t j = 0; j < cols; j++)
        {
            // 当前二维下标
            size_t indices[2] = {i, j};

            // 用来保存读取到的值
            int value;

            // 读取 arr[i][j]
            if (ndarray_get(arr, indices, &value))
            {
                printf("%d\t", value);
            }
        }

        // 一行打印完后换行
        printf("\n");
    }
}

/*
    改变数组形状 reshape

    注意：
        reshape 不会改变底层 data 的内容。
        它只是改变 dims 和 strides，也就是改变解释数据的方式。

    要求：
        新形状的总元素数量必须等于原来的 total。

    例如：
        原来是 3 x 4，总数 12
        可以 reshape 成 2 x 6，总数也是 12

        但是不能 reshape 成 5 x 5，因为总数变成 25。
*/
int ndarray_reshape(NDArray *arr, int new_ndim, const size_t new_dims[])
{
    // 参数检查
    if (arr == NULL || new_ndim <= 0 || new_dims == NULL)
    {
        return 0;
    }

    // 计算新形状的总元素数量
    size_t new_total = 1;

    for (int i = 0; i < new_ndim; i++)
    {
        // 每一维长度不能为 0
        if (new_dims[i] == 0)
        {
            return 0;
        }

        // 防止乘法溢出
        if (new_total > SIZE_MAX / new_dims[i])
        {
            return 0;
        }

        // 累乘得到新总元素数量
        new_total *= new_dims[i];
    }

    /*
        reshape 的前提：
        新旧总元素数量必须相同。
    */
    if (new_total != arr->total)
    {
        return 0;
    }

    // 给新的 dims 分配空间
    size_t *new_dims_copy = malloc(sizeof(size_t) * new_ndim);

    // 给新的 strides 分配空间
    size_t *new_strides = malloc(sizeof(size_t) * new_ndim);

    // 如果任意一个分配失败，就释放已经分配的内存
    if (new_dims_copy == NULL || new_strides == NULL)
    {
        free(new_dims_copy);
        free(new_strides);
        return 0;
    }

    // 复制新的维度信息
    for (int i = 0; i < new_ndim; i++)
    {
        new_dims_copy[i] = new_dims[i];
    }

    // 重新计算新的 strides
    new_strides[new_ndim - 1] = 1;

    for (int i = new_ndim - 2; i >= 0; i--)
    {
        new_strides[i] = new_strides[i + 1] * new_dims_copy[i + 1];
    }

    // 释放旧的 dims 和 strides
    free(arr->dims);
    free(arr->strides);

    // 替换成新的 dims 和 strides
    arr->dims = new_dims_copy;
    arr->strides = new_strides;
    arr->ndim = new_ndim;

    // reshape 成功
    return 1;
}

/*
    测试代码
*/
int main(void)
{
    /*
        创建一个二维数组：
            3 行 4 列
    */
    size_t dims[2] = {3, 4};

    // 创建 NDArray
    NDArray *matrix = ndarray_create(2, dims);

    // 检查是否创建成功
    if (matrix == NULL)
    {
        printf("创建数组失败\n");
        return 1;
    }

    /*
        设置几个元素

        等价于：
            matrix[0][0] = 10;
            matrix[1][2] = 20;
            matrix[2][3] = 30;
    */
    ndarray_set(matrix, (size_t[]){0, 0}, 10);
    ndarray_set(matrix, (size_t[]){1, 2}, 20);
    ndarray_set(matrix, (size_t[]){2, 3}, 30);

    // 打印数组的维度信息
    ndarray_print_info(matrix);

    // 按二维矩阵形式打印数组内容
    printf("二维数组内容：\n");
    ndarray_print_2d(matrix);

    /*
        读取 matrix[1][2]
    */
    int value;

    if (ndarray_get(matrix, (size_t[]){1, 2}, &value))
    {
        printf("matrix[1][2] = %d\n", value);
    }
    else
    {
        printf("读取失败\n");
    }

    /*
        reshape 示例

        原来是：
            3 x 4 = 12

        改成：
            2 x 6 = 12

        因为总元素数量一样，所以可以 reshape。
    */
    size_t new_dims[2] = {2, 6};

    if (ndarray_reshape(matrix, 2, new_dims))
    {
        printf("reshape 成功\n");
        ndarray_print_info(matrix);
        ndarray_print_2d(matrix);
    }
    else
    {
        printf("reshape 失败\n");
    }

    // 释放数组
    ndarray_free(matrix);

    return 0;
}