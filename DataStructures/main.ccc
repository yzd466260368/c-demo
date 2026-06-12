#include <stdio.h>
#include <stdlib.h>

// 队列节点
typedef struct Node
{
    int data;
    struct Node *next;
} Node;

// 队列
typedef struct
{
    Node *front; // 队头
    Node *rear;  // 队尾
    int size;    // 当前元素数量
} Queue;

Queue *InitQueue();
void ReleaseQueue(Queue *q);

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
    printf("front = %d\n", front(q));
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

    q->front = NULL;
    q->rear = NULL;
    q->size = 0;

    return q;
}

void ReleaseQueue(Queue *q)
{
    if (q == NULL)
    {
        return;
    }

    Node *cur = q->front;

    while (cur != NULL)
    {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }

    free(q);
}

int is_queue_empty(Queue *q)
{
    return q == NULL || q->size == 0;
}

void enqueue(Queue *q, int value)
{
    if (q == NULL)
    {
        return;
    }

    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
    {
        printf("内存分配失败\n");
        return;
    }

    node->data = value;
    node->next = NULL;

    // 情况 1：队列为空
    if (q->rear == NULL)
    {
        q->front = node;
        q->rear = node;
    }
    // 情况 2：队列不为空
    else
    {
        q->rear->next = node;
        q->rear = node;
    }

    q->size++;
}

int dequeue(Queue *q)
{
    if (is_queue_empty(q))
    {
        printf("队列为空，无法 dequeue\n");
        exit(EXIT_FAILURE);
    }

    Node *node = q->front;
    int value = node->data;

    q->front = q->front->next;

    // 如果出队后队列为空，需要把 rear 也置为 NULL
    if (q->front == NULL)
    {
        q->rear = NULL;
    }

    free(node);
    q->size--;

    return value;
}

int front(Queue *q)
{
    if (is_queue_empty(q))
    {
        printf("队列为空，无法 front\n");
        exit(EXIT_FAILURE);
    }

    return q->front->data;
}