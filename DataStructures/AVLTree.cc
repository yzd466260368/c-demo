#include <stdio.h>
#include <stdlib.h>

/*
    AVL 树节点结构
*/
typedef struct Node
{
    int key;            // 节点存储的数据
    int height;         // 当前节点的高度
    struct Node *left;  // 左孩子
    struct Node *right; // 右孩子
} Node;

/*
    获取两个数中的较大值
*/
int max(int a, int b)
{
    return a > b ? a : b;
}

/*
    获取节点高度
    空节点高度为 0
*/
int height(Node *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return node->height;
}

/*
    创建一个新节点
*/
Node *createNode(int key)
{
    Node *node = (Node *)malloc(sizeof(Node));

    if (node == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }

    node->key = key;
    node->height = 1; // 新节点高度为 1
    node->left = NULL;
    node->right = NULL;

    return node;
}

/*
    获取平衡因子
    平衡因子 = 左子树高度 - 右子树高度
*/
int getBalance(Node *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return height(node->left) - height(node->right);
}

/*
    右旋

        y
       /
      x
       \
        T2

    右旋后：

        x
         \
          y
         /
        T2
*/
Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    // 开始旋转
    x->right = y;
    y->left = T2;

    // 更新高度，注意先更新 y，再更新 x
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // 返回新的根节点
    return x;
}

/*
    左旋

      x
       \
        y
       /
      T2

    左旋后：

        y
       /
      x
       \
        T2
*/
Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    // 开始旋转
    y->left = x;
    x->right = T2;

    // 更新高度，注意先更新 x，再更新 y
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // 返回新的根节点
    return y;
}

/*
    插入节点，并保持 AVL 树平衡
*/
Node *insert(Node *node, int key)
{
    /*
        第一步：按照普通二叉搜索树的规则插入
    */
    if (node == NULL)
    {
        return createNode(key);
    }

    if (key < node->key)
    {
        node->left = insert(node->left, key);
    }
    else if (key > node->key)
    {
        node->right = insert(node->right, key);
    }
    else
    {
        // 不允许插入重复值
        return node;
    }

    /*
        第二步：更新当前节点高度
    */
    node->height = max(height(node->left), height(node->right)) + 1;

    /*
        第三步：计算平衡因子
    */
    int balance = getBalance(node);

    /*
        第四步：判断是否失衡，并进行旋转
    */

    // 情况 1：左左情况 LL
    if (balance > 1 && key < node->left->key)
    {
        return rightRotate(node);
    }

    // 情况 2：右右情况 RR
    if (balance < -1 && key > node->right->key)
    {
        return leftRotate(node);
    }

    // 情况 3：左右情况 LR
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // 情况 4：右左情况 RL
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /*
        如果没有失衡，直接返回当前节点
    */
    return node;
}

/*
    查找节点
*/
Node *search(Node *root, int key)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (key == root->key)
    {
        return root;
    }
    else if (key < root->key)
    {
        return search(root->left, key);
    }
    else
    {
        return search(root->right, key);
    }
}

/*
    中序遍历
    AVL 树本质是二叉搜索树，所以中序遍历结果是从小到大
*/
void inorder(Node *root)
{
    if (root == NULL)
    {
        return;
    }

    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

/*
    前序遍历
    可以用来看树的大致结构
*/
void preorder(Node *root)
{
    if (root == NULL)
    {
        return;
    }

    printf("%d ", root->key);
    preorder(root->left);
    preorder(root->right);
}

/*
    释放整棵树
*/
void freeTree(Node *root)
{
    if (root == NULL)
    {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main()
{
    Node *root = NULL;

    int nums[] = {10, 20, 30, 40, 50, 25};
    int n = sizeof(nums) / sizeof(nums[0]);

    for (int i = 0; i < n; i++)
    {
        root = insert(root, nums[i]);
    }

    printf("中序遍历结果：");
    inorder(root);
    printf("\n");

    printf("前序遍历结果：");
    preorder(root);
    printf("\n");

    int target = 25;
    Node *result = search(root, target);

    if (result != NULL)
    {
        printf("找到了节点：%d\n", target);
    }
    else
    {
        printf("没有找到节点：%d\n", target);
    }

    freeTree(root);

    return 0;
}