// #include <stdio.h>
// #include <stdlib.h>

// typedef struct Node
// {
//     int data;
//     struct Node *next;
// } Node;

// void joseph(int n, int k, int m)
// {
//     if (n <= 0 || k < 1 || k > n || m <= 0)
//     {
//         return;
//     }

//     Node *head = NULL, *tail = NULL;

//     // 1. 初始化无头单向循环链表：1, 2, ..., n
//     for (int i = 1; i <= n; i++)
//     {
//         Node *p = (Node *)malloc(sizeof(Node));
//         p->data = i;
//         p->next = NULL;

//         if (head == NULL)
//         {
//             head = p;
//             tail = p;
//         }
//         else
//         {
//             tail->next = p;
//             tail = p;
//         }
//     }

//     tail->next = head; // 构成循环链表

//     // 2. 找到编号为 k 的结点，作为第一次开始报数的人
//     Node *pre = tail;
//     Node *cur = head;

//     for (int i = 1; i < k; i++)
//     {
//         pre = cur;
//         cur = cur->next;
//     }

//     printf("出队序列为：(");

//     // 3. 依次报数出队
//     int count = n;
//     while (count > 0)
//     {
//         // 从 cur 开始数 1，数到 m
//         for (int i = 1; i < m; i++)
//         {
//             pre = cur;
//             cur = cur->next;
//         }

//         // cur 即为要出队的人
//         printf("%d", cur->data);
//         if (count > 1)
//         {
//             printf(", ");
//         }

//         // 删除 cur
//         pre->next = cur->next;
//         Node *temp = cur;
//         cur = cur->next;
//         free(temp);

//         count--;
//     }

//     printf(")。\n");
// }

// int main()
// {
//     joseph(8, 3, 4);
//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>

// #define HASH_SIZE 10

// // 哈希表结点
// typedef struct Node
// {
//     int key;
//     struct Node *next;
// } Node;

// // 哈希表
// typedef struct HashTable
// {
//     Node *table[HASH_SIZE];
// } HashTable;

// // 哈希函数：除留余数法
// int hash(int key)
// {
//     return key % HASH_SIZE;
// }

// // 初始化哈希表
// void initHashTable(HashTable *ht)
// {
//     for (int i = 0; i < HASH_SIZE; i++)
//     {
//         ht->table[i] = NULL;
//     }
// }

// // 插入关键字
// void insertHashTable(HashTable *ht, int key)
// {
//     int index = hash(key);

//     Node *p = (Node *)malloc(sizeof(Node));
//     if (p == NULL)
//     {
//         printf("内存分配失败！\n");
//         return;
//     }

//     p->key = key;

//     // 头插法插入到对应链表中
//     p->next = ht->table[index];
//     ht->table[index] = p;
// }

// // 查询关键字
// Node *searchHashTable(HashTable *ht, int key)
// {
//     int index = hash(key);

//     Node *p = ht->table[index];
//     while (p != NULL)
//     {
//         if (p->key == key)
//         {
//             return p;
//         }
//         p = p->next;
//     }

//     return NULL;
// }

// // 输出哈希表
// void printHashTable(HashTable *ht)
// {
//     for (int i = 0; i < HASH_SIZE; i++)
//     {
//         printf("%d: ", i);

//         Node *p = ht->table[i];
//         while (p != NULL)
//         {
//             printf("%d -> ", p->key);
//             p = p->next;
//         }

//         printf("NULL\n");
//     }
// }

// // 释放哈希表
// void destroyHashTable(HashTable *ht)
// {
//     for (int i = 0; i < HASH_SIZE; i++)
//     {
//         Node *p = ht->table[i];

//         while (p != NULL)
//         {
//             Node *temp = p;
//             p = p->next;
//             free(temp);
//         }

//         ht->table[i] = NULL;
//     }
// }

// int main()
// {
//     HashTable ht;

//     initHashTable(&ht);

//     insertHashTable(&ht, 12);
//     insertHashTable(&ht, 22);
//     insertHashTable(&ht, 35);
//     insertHashTable(&ht, 42);
//     insertHashTable(&ht, 15);

//     printHashTable(&ht);

//     int key = 22;
//     Node *result = searchHashTable(&ht, key);

//     if (result != NULL)
//     {
//         printf("查找成功，关键字 %d 存在。\n", key);
//     }
//     else
//     {
//         printf("查找失败，关键字 %d 不存在。\n", key);
//     }

//     destroyHashTable(&ht);

//     return 0;
// }

// 数组实现
// #include <stdio.h>
// #include <stdlib.h>

// #define MAX_SIZE 100

// typedef struct
// {
//     int data[MAX_SIZE]; // 存储完全二叉树结点
//     int size;           // 当前结点个数
// } CompleteBinaryTree;

// // 初始化完全二叉树
// void initTree(CompleteBinaryTree *tree)
// {
//     tree->size = 0;
// }

// // 插入结点
// int insertNode(CompleteBinaryTree *tree, int value)
// {
//     if (tree->size >= MAX_SIZE)
//     {
//         return 0; // 插入失败，树已满
//     }

//     tree->data[tree->size] = value;
//     tree->size++;

//     return 1; // 插入成功
// }

// // 获取左孩子下标
// int getLeftChild(int index)
// {
//     return 2 * index + 1;
// }

// // 获取右孩子下标
// int getRightChild(int index)
// {
//     return 2 * index + 2;
// }

// // 获取父结点下标
// int getParent(int index)
// {
//     if (index == 0)
//     {
//         return -1; // 根结点没有父结点
//     }
//     return (index - 1) / 2;
// }

// // 层序遍历输出
// void printTree(CompleteBinaryTree *tree)
// {
//     for (int i = 0; i < tree->size; i++)
//     {
//         printf("%d ", tree->data[i]);
//     }
//     printf("\n");
// }

// int main()
// {
//     CompleteBinaryTree tree;

//     initTree(&tree);

//     insertNode(&tree, 1);
//     insertNode(&tree, 2);
//     insertNode(&tree, 3);
//     insertNode(&tree, 4);
//     insertNode(&tree, 5);
//     insertNode(&tree, 6);

//     printf("完全二叉树层序遍历结果：");
//     printTree(&tree);

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>

// 二叉树结点
typedef struct TreeNode
{
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 完全二叉树
typedef struct
{
    TreeNode *root;
    int size;
} CompleteBinaryTree;

// 队列结点
typedef struct QueueNode
{
    TreeNode *treeNode;
    struct QueueNode *next;
} QueueNode;

// 链式队列
typedef struct
{
    QueueNode *front;
    QueueNode *rear;
} Queue;

// 初始化完全二叉树
void initTree(CompleteBinaryTree *tree)
{
    tree->root = NULL;
    tree->size = 0;
}

// 创建二叉树结点
TreeNode *createTreeNode(int value)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));

    if (node == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }

    node->data = value;
    node->left = NULL;
    node->right = NULL;

    return node;
}

// 初始化队列
void initQueue(Queue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

// 判断队列是否为空
int isQueueEmpty(Queue *queue)
{
    return queue->front == NULL;
}

// 入队
void enqueue(Queue *queue, TreeNode *treeNode)
{
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));

    if (newNode == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }

    newNode->treeNode = treeNode;
    newNode->next = NULL;

    // 如果队列为空
    if (queue->rear == NULL)
    {
        queue->front = newNode;
        queue->rear = newNode;
    }
    else
    {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// 出队
TreeNode *dequeue(Queue *queue)
{
    if (isQueueEmpty(queue))
    {
        return NULL;
    }

    QueueNode *temp = queue->front;
    TreeNode *treeNode = temp->treeNode;

    queue->front = queue->front->next;

    // 如果出队后队列为空，rear 也要置空
    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }

    free(temp);

    return treeNode;
}

// 清空队列
// 注意：这里只释放队列结点 QueueNode
// 不释放二叉树结点 TreeNode
void clearQueue(Queue *queue)
{
    while (!isQueueEmpty(queue))
    {
        dequeue(queue);
    }
}

// 插入结点，保持完全二叉树性质
void insertNode(CompleteBinaryTree *tree, int value)
{
    TreeNode *newNode = createTreeNode(value);

    // 如果是空树，新结点直接作为根结点
    if (tree->root == NULL)
    {
        tree->root = newNode;
        tree->size++;
        return;
    }

    Queue queue;
    initQueue(&queue);

    // 从根结点开始层序查找
    enqueue(&queue, tree->root);

    while (!isQueueEmpty(&queue))
    {
        TreeNode *current = dequeue(&queue);

        // 如果当前结点没有左孩子，插到左边
        if (current->left == NULL)
        {
            current->left = newNode;
            tree->size++;

            // 插入完成前，释放队列中剩余的 QueueNode
            clearQueue(&queue);
            return;
        }
        else
        {
            enqueue(&queue, current->left);
        }

        // 如果当前结点没有右孩子，插到右边
        if (current->right == NULL)
        {
            current->right = newNode;
            tree->size++;

            // 插入完成前，释放队列中剩余的 QueueNode
            clearQueue(&queue);
            return;
        }
        else
        {
            enqueue(&queue, current->right);
        }
    }
}

// 层序遍历输出
void levelOrderTraversal(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }

    Queue queue;
    initQueue(&queue);

    enqueue(&queue, root);

    while (!isQueueEmpty(&queue))
    {
        TreeNode *current = dequeue(&queue);

        printf("%d ", current->data);

        if (current->left != NULL)
        {
            enqueue(&queue, current->left);
        }

        if (current->right != NULL)
        {
            enqueue(&queue, current->right);
        }
    }

    // 正常情况下队列已经空了
    // 这里写上也没有问题，作为保险
    clearQueue(&queue);
}

// 释放整棵二叉树
void destroyTree(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }

    destroyTree(root->left);
    destroyTree(root->right);

    free(root);
}

int main()
{
    CompleteBinaryTree tree;

    initTree(&tree);

    insertNode(&tree, 1);
    insertNode(&tree, 2);
    insertNode(&tree, 3);
    insertNode(&tree, 4);
    insertNode(&tree, 5);
    insertNode(&tree, 6);
    insertNode(&tree, 7);

    printf("层序遍历结果：");
    levelOrderTraversal(tree.root);

    printf("\n结点个数：%d\n", tree.size);

    destroyTree(tree.root);
    tree.root = NULL;
    tree.size = 0;

    return 0;
}
