#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int key;
    int height;
    struct Node *left;
    struct Node *right;
} Node;

int max(int a, int b)
{
    return a > b ? a : b;
}

/* 空节点高度为 0 */
int height(Node *node)
{
    return node == NULL ? 0 : node->height;
}

/* 平衡因子 = 左子树高度 - 右子树高度 */
int getBalance(Node *node)
{
    return node == NULL ? 0 : height(node->left) - height(node->right);
}

/* 创建新节点 */
Node *createNode(int key)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* 更新节点高度 */
void updateHeight(Node *node)
{
    node->height = max(height(node->left), height(node->right)) + 1;
}

/*
    右旋：处理 LL 型失衡

          y
         /
        x
       /
      T1

    右旋后：

        x
       / \
      T1  y
*/
Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    printf("触发右旋：以 %d 为根进行右旋\n", y->key);

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

/*
    左旋：处理 RR 型失衡

      x
       \
        y
         \
          T3

    左旋后：

        y
       / \
      x   T3
*/
Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    printf("触发左旋：以 %d 为根进行左旋\n", x->key);

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

/* 插入节点 */
Node *insert(Node *root, int key)
{
    if (root == NULL)
    {
        printf("插入节点：%d\n", key);
        return createNode(key);
    }

    if (key < root->key)
    {
        root->left = insert(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = insert(root->right, key);
    }
    else
    {
        printf("节点 %d 已存在，忽略插入\n", key);
        return root;
    }

    updateHeight(root);

    int balance = getBalance(root);

    /*
        LL 型：
        当前节点左边太高，并且新 key 插入到了左孩子的左边
    */
    if (balance > 1 && key < root->left->key)
    {
        return rightRotate(root);
    }

    /*
        RR 型：
        当前节点右边太高，并且新 key 插入到了右孩子的右边
    */
    if (balance < -1 && key > root->right->key)
    {
        return leftRotate(root);
    }

    /*
        LR 型：
        当前节点左边太高，但新 key 插入到了左孩子的右边
        先对左孩子左旋，再对当前节点右旋
    */
    if (balance > 1 && key > root->left->key)
    {
        printf("触发 LR 型失衡：先左旋，再右旋\n");
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    /*
        RL 型：
        当前节点右边太高，但新 key 插入到了右孩子的左边
        先对右孩子右旋，再对当前节点左旋
    */
    if (balance < -1 && key < root->right->key)
    {
        printf("触发 RL 型失衡：先右旋，再左旋\n");
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

/* 找到当前子树中最小的节点，用于删除 */
Node *minValueNode(Node *node)
{
    Node *current = node;

    while (current->left != NULL)
    {
        current = current->left;
    }

    return current;
}

/* 删除节点 */
Node *deleteNode(Node *root, int key)
{
    if (root == NULL)
    {
        return root;
    }

    if (key < root->key)
    {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = deleteNode(root->right, key);
    }
    else
    {
        printf("删除节点：%d\n", key);

        if (root->left == NULL || root->right == NULL)
        {
            Node *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
            {
                *root = *temp;
            }

            free(temp);
        }
        else
        {
            Node *temp = minValueNode(root->right);

            root->key = temp->key;

            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL)
    {
        return root;
    }

    updateHeight(root);

    int balance = getBalance(root);

    /*
        删除后也可能失衡。
        与插入不同，这里通过子树的 balance 判断旋转类型。
    */

    // LL 型
    if (balance > 1 && getBalance(root->left) >= 0)
    {
        return rightRotate(root);
    }

    // LR 型
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RR 型
    if (balance < -1 && getBalance(root->right) <= 0)
    {
        return leftRotate(root);
    }

    // RL 型
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

/* 查找节点 */
Node *search(Node *root, int key)
{
    if (root == NULL || root->key == key)
    {
        return root;
    }

    if (key < root->key)
    {
        return search(root->left, key);
    }

    return search(root->right, key);
}

/* 中序遍历：结果应该是从小到大 */
void inorder(Node *root)
{
    if (root == NULL)
        return;

    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

/*
    横向打印树结构
    右子树在上，左子树在下
*/
void printTree(Node *root, int space)
{
    if (root == NULL)
        return;

    space += 5;

    printTree(root->right, space);

    printf("\n");
    for (int i = 5; i < space; i++)
    {
        printf(" ");
    }

    printf("%d(h=%d,b=%d)\n",
           root->key,
           root->height,
           getBalance(root));

    printTree(root->left, space);
}

/* 释放整棵树 */
void freeTree(Node *root)
{
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main()
{
    Node *root = NULL;

    /*
        这组数据可以触发多种旋转：
        30, 20, 10 触发 LL，右旋
        40, 50 可能触发 RR，左旋
        25, 28 可观察 LR/RL 调整
    */
    int nums[] = {30, 20, 10, 40, 50, 25, 28, 27, 5, 4};
    int n = sizeof(nums) / sizeof(nums[0]);

    printf("========== AVL 插入测试 ==========\n");

    for (int i = 0; i < n; i++)
    {
        printf("\n--- 插入 %d ---\n", nums[i]);
        root = insert(root, nums[i]);

        printf("当前 AVL 树：\n");
        printTree(root, 0);
        printf("\n");
    }

    printf("\n========== 中序遍历 ==========\n");
    inorder(root);
    printf("\n");

    printf("\n========== 查找测试 ==========\n");
    int target = 25;
    Node *found = search(root, target);

    if (found)
    {
        printf("找到节点 %d，高度=%d，平衡因子=%d\n",
               found->key,
               found->height,
               getBalance(found));
    }
    else
    {
        printf("没有找到节点 %d\n", target);
    }

    printf("\n========== 删除测试 ==========\n");

    int deletes[] = {50, 40, 30};
    int dCount = sizeof(deletes) / sizeof(deletes[0]);

    for (int i = 0; i < dCount; i++)
    {
        printf("\n--- 删除 %d ---\n", deletes[i]);
        root = deleteNode(root, deletes[i]);

        printf("删除后的 AVL 树：\n");
        printTree(root, 0);
        printf("\n");
    }

    freeTree(root);

    return 0;
}