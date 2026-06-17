#include <stdio.h>
#include <stdlib.h>

#define CAPACITY 10
#define RESIZE 10

// 队列
typedef struct
{
    int *data;
    int front;    // 队头
    int rear;     // 队尾的下一个位置
    int size;     // 当前元素数量
    int capacity; // 容量
} Queue;

Queue *InitQueue();
void ReleaseQueue(Queue *q);
int ResizeQueue(Queue *q);

void enqueue(Queue *q, int value);
int dequeue(Queue *q);
int front(Queue *q);
int is_queue_empty(Queue *q);

int main(int argc, char const *argv[])
{
    Queue *q = InitQueue();

    enqueue(q, 10);
    enqueue(q, 20);
    enqueue(q, 30);

    printf("front = %d\n", front(q));
    printf("dequeue = %d\n", dequeue(q));
    printf("dequeue = %d\n", dequeue(q));

    enqueue(q, 40);

    printf("dequeue = %d\n", dequeue(q));
    printf("dequeue = %d\n", dequeue(q));

    ReleaseQueue(q);

    return 0;
}

Queue *InitQueue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (q == NULL)
    {
        return NULL;
    }

    q->data = (int *)malloc(CAPACITY * sizeof(int));
    if (q->data == NULL)
    {
        free(q);
        return NULL;
    }

    q->front = 0;
    q->rear = 0;
    q->size = 0;
    q->capacity = CAPACITY;

    return q;
}

void ReleaseQueue(Queue *q)
{
    if (q == NULL)
    {
        return;
    }

    free(q->data);
    free(q);
}

int ResizeQueue(Queue *q)
{
    if (q == NULL)
    {
        return 0;
    }

    int new_capacity = q->capacity + RESIZE;
    int *new_data = (int *)malloc(new_capacity * sizeof(int));

    if (new_data == NULL)
    {
        return 0;
    }

    // 把旧队列中的元素按正确顺序复制到新数组
    for (int i = 0; i < q->size; i++)
    {
        new_data[i] = q->data[(q->front + i) % q->capacity];
    }

    free(q->data);

    q->data = new_data;
    q->capacity = new_capacity;
    q->front = 0;
    q->rear = q->size;

    return 1;
}

void enqueue(Queue *q, int value)
{
    if (q == NULL)
    {
        return;
    }

    if (q->size == q->capacity)
    {
        if (!ResizeQueue(q))
        {
            printf("扩容失败\n");
            return;
        }
    }

    q->data[q->rear] = value;
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;
}

int dequeue(Queue *q)
{
    if (q == NULL || is_queue_empty(q))
    {
        printf("队列为空，无法 dequeue\n");
        exit(EXIT_FAILURE);
    }

    int value = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;

    return value;
}

int front(Queue *q)
{
    if (q == NULL || is_queue_empty(q))
    {
        printf("队列为空，无法 front\n");
        exit(EXIT_FAILURE);
    }

    return q->data[q->front];
}

int is_queue_empty(Queue *q)
{
    return q == NULL || q->size == 0;
}