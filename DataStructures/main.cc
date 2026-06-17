#include <stdio.h>

/*C 语言的数据结构基本都是靠 结构体 + 指针 + malloc/free 搭出来的。*/
// 动态数组
typedef struct
{
    int *data;
    int size;
    int capacity;
} IntArrayList;
// 链表
typedef struct Node
{
    int data;
    struct Node *next;
} Node;
// 栈
typedef struct
{
    int *data;
    int top;      // 栈顶下标，也可以表示元素个数
    int capacity; // 容量
} Stack;

void push(Stack *s, int value);
int pop(Stack *s);
int peek(Stack *s);
int is_stack_empty(Stack *s);

// 队列
typedef struct
{
    int *data;
    int front;    // 队头
    int rear;     // 队尾的下一个位置
    int size;     // 当前元素数量
    int capacity; // 容量
} Queue;

void enqueue(Queue *q, int value);
int dequeue(Queue *q);
int front(Queue *q);
int is_queue_empty(Queue *q);

// 二叉树
typedef struct TreeNode
{
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 哈希表
typedef struct HashNode
{
    int key;
    int value;
    struct HashNode *next;
} HashNode;

int main(int argc, char const *argv[])
{

    return 0;
}
