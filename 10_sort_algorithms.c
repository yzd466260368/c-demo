#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_array(const int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void copy_array(const int *src, int *dst, int n)
{
    memcpy(dst, src, n * sizeof(int));
}

void bubble_sort(int *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void selection_sort(int *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[min_idx])
            {
                min_idx = j;
            }
        }
        if (min_idx != i)
        {
            int tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
}

void insertion_sort(int *arr, int n)
{
    for (int i = 1; i < n; i++)
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

void shell_sort(int *arr, int n)
{
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp)
            {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}

void merge(int *arr, int left, int mid, int right, int *tmp)
{
    int i = left;
    int j = mid + 1;
    int k = left;
    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
        {
            tmp[k++] = arr[i++];
        }
        else
        {
            tmp[k++] = arr[j++];
        }
    }
    while (i <= mid)
    {
        tmp[k++] = arr[i++];
    }
    while (j <= right)
    {
        tmp[k++] = arr[j++];
    }
    for (int p = left; p <= right; p++)
    {
        arr[p] = tmp[p];
    }
}

void merge_sort_recursive(int *arr, int left, int right, int *tmp)
{
    if (left >= right)
    {
        return;
    }
    int mid = left + (right - left) / 2;
    merge_sort_recursive(arr, left, mid, tmp);
    merge_sort_recursive(arr, mid + 1, right, tmp);
    merge(arr, left, mid, right, tmp);
}

void merge_sort(int *arr, int n)
{
    int *tmp = malloc(n * sizeof(int));
    if (!tmp)
    {
        return;
    }
    merge_sort_recursive(arr, 0, n - 1, tmp);
    free(tmp);
}

int partition(int *arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    int tmp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = tmp;
    return i + 1;
}

void quick_sort_recursive(int *arr, int low, int high)
{
    if (low < high)
    {
        int pivot_index = partition(arr, low, high);
        quick_sort_recursive(arr, low, pivot_index - 1);
        quick_sort_recursive(arr, pivot_index + 1, high);
    }
}

void quick_sort(int *arr, int n)
{
    quick_sort_recursive(arr, 0, n - 1);
}

void heapify(int *arr, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest])
    {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest])
    {
        largest = right;
    }
    if (largest != i)
    {
        int tmp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = tmp;
        heapify(arr, n, largest);
    }
}

void heap_sort(int *arr, int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i > 0; i--)
    {
        int tmp = arr[0];
        arr[0] = arr[i];
        arr[i] = tmp;
        heapify(arr, i, 0);
    }
}

void counting_sort(int *arr, int n)
{
    if (n <= 0)
    {
        return;
    }
    int max_val = arr[0];
    for (int i = 1; i < n; i++)
    {
        if (arr[i] > max_val)
        {
            max_val = arr[i];
        }
    }
    int *count = calloc(max_val + 1, sizeof(int));
    if (!count)
    {
        return;
    }
    for (int i = 0; i < n; i++)
    {
        count[arr[i]]++;
    }
    int index = 0;
    for (int value = 0; value <= max_val; value++)
    {
        while (count[value] > 0)
        {
            arr[index++] = value;
            count[value]--;
        }
    }
    free(count);
}

int get_max(int *arr, int n)
{
    int max_val = arr[0];
    for (int i = 1; i < n; i++)
    {
        if (arr[i] > max_val)
        {
            max_val = arr[i];
        }
    }
    return max_val;
}

void radix_sort(int *arr, int n)
{
    if (n <= 0)
    {
        return;
    }
    int max_val = get_max(arr, n);
    int *tmp = malloc(n * sizeof(int));
    if (!tmp)
    {
        return;
    }
    for (int exp = 1; max_val / exp > 0; exp *= 10)
    {
        int bucket[10] = {0};
        for (int i = 0; i < n; i++)
        {
            bucket[(arr[i] / exp) % 10]++;
        }
        for (int i = 1; i < 10; i++)
        {
            bucket[i] += bucket[i - 1];
        }
        for (int i = n - 1; i >= 0; i--)
        {
            int digit = (arr[i] / exp) % 10;
            tmp[--bucket[digit]] = arr[i];
        }
        for (int i = 0; i < n; i++)
        {
            arr[i] = tmp[i];
        }
    }
    free(tmp);
}

void bucket_sort(int *arr, int n)
{
    if (n <= 0)
    {
        return;
    }
    int max_val = get_max(arr, n);
    int bucket_count = n;
    int bucket_size = bucket_count;
    int **buckets = malloc(bucket_count * sizeof(int *));
    int *counts = calloc(bucket_count, sizeof(int));
    if (!buckets || !counts)
    {
        free(buckets);
        free(counts);
        return;
    }
    for (int i = 0; i < bucket_count; i++)
    {
        buckets[i] = calloc(bucket_size, sizeof(int));
        if (!buckets[i])
        {
            for (int j = 0; j < i; j++)
            {
                free(buckets[j]);
            }
            free(buckets);
            free(counts);
            return;
        }
    }
    for (int i = 0; i < n; i++)
    {
        int idx = (arr[i] * bucket_count) / (max_val + 1);
        if (idx >= bucket_count)
        {
            idx = bucket_count - 1;
        }
        buckets[idx][counts[idx]++] = arr[i];
    }
    int index = 0;
    for (int i = 0; i < bucket_count; i++)
    {
        for (int j = 1; j < counts[i]; j++)
        {
            int key = buckets[i][j];
            int k = j - 1;
            while (k >= 0 && buckets[i][k] > key)
            {
                buckets[i][k + 1] = buckets[i][k];
                k--;
            }
            buckets[i][k + 1] = key;
        }
        for (int j = 0; j < counts[i]; j++)
        {
            arr[index++] = buckets[i][j];
        }
        free(buckets[i]);
    }
    free(buckets);
    free(counts);
}

int main(void)
{
    int original[] = {29, 10, 14, 37, 13, 52, 46, 41, 58, 22};
    int n = sizeof(original) / sizeof(original[0]);
    int arr[sizeof(original) / sizeof(original[0])];

    printf("原始数组: ");
    print_array(original, n);

    copy_array(original, arr, n);
    bubble_sort(arr, n);
    printf("冒泡排序: ");
    print_array(arr, n);

    copy_array(original, arr, n);
    selection_sort(arr, n);
    printf("选择排序: ");
    print_array(arr, n);

    copy_array(original, arr, n);
    insertion_sort(arr, n);
    printf("插入排序: ");
    print_array(arr, n);

    copy_array(original, arr, n);
    shell_sort(arr, n);
    printf("希尔排序: ");
    print_array(arr, n);

    copy_array(original, arr, n);
    merge_sort(arr, n);
    printf("归并排序: ");
    print_array(arr, n);

    copy_array(original, arr, n);
    quick_sort(arr, n);
    printf("快速排序: ");
    print_array(arr, n);

    copy_array(original, arr, n);
    heap_sort(arr, n);
    printf("堆排序: ");
    print_array(arr, n);

    copy_array(original, arr, n);
    counting_sort(arr, n);
    printf("计数排序: ");
    print_array(arr, n);

    copy_array(original, arr, n);
    radix_sort(arr, n);
    printf("基数排序: ");
    print_array(arr, n);

    copy_array(original, arr, n);
    bucket_sort(arr, n);
    printf("桶排序: ");
    print_array(arr, n);

    return 0;
}
