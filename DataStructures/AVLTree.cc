#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
    AVL 平衡二叉树节点结构
*/
typedef struct AVLNode
{
    int key;               // 节点数据
    int height;            // 节点高度
    struct AVLNode *left;  // 左孩子
    struct AVLNode *right; // 右孩子
} AVLNode;

/*
    AVL 树管理结构
*/
typedef struct AVLTree
{
    AVLNode *root; // 根节点
    int size;      // 节点数量
} AVLTree;

/*
    栈结构，用于非递归遍历
*/
typedef struct Stack
{
    AVLNode **data;
    int top;
    int capacity;
} Stack;

/* =========================
   一、栈的基本操作
   ========================= */

void initStack(Stack *stack)
{
    stack->capacity = 100;
    stack->top = -1;
    stack->data = (AVLNode **)malloc(sizeof(AVLNode *) * stack->capacity);

    if (stack->data == NULL)
    {
        printf("栈内存分配失败\n");
        exit(1);
    }
}

bool isStackEmpty(Stack *stack)
{
    return stack->top == -1;
}

void push(Stack *stack, AVLNode *node)
{
    if (stack->top + 1 >= stack->capacity)
    {
        stack->capacity *= 2;
        stack->data = (AVLNode **)realloc(stack->data, sizeof(AVLNode *) * stack->capacity);

        if (stack->data == NULL)
        {
            printf("栈扩容失败\n");
            exit(1);
        }
    }

    stack->data[++stack->top] = node;
}

AVLNode *pop(Stack *stack)
{
    if (isStackEmpty(stack))
    {
        return NULL;
    }

    return stack->data[stack->top--];
}

AVLNode *peek(Stack *stack)
{
    if (isStackEmpty(stack))
    {
        return NULL;
    }

    return stack->data[stack->top];
}

void destroyStack(Stack *stack)
{
    free(stack->data);
    stack->data = NULL;
    stack->top = -1;
    stack->capacity = 0;
}

/* =========================
   二、AVL 树基础工具函数
   ========================= */

int max(int a, int b)
{
    return a > b ? a : b;
}

/*
    获取节点高度
    空节点高度为 0
*/
int getHeight(AVLNode *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return node->height;
}

/*
    更新节点高度
*/
void updateHeight(AVLNode *node)
{
    if (node != NULL)
    {
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    }
}

/*
    获取平衡因子
    平衡因子 = 左子树高度 - 右子树高度
*/
int getBalanceFactor(AVLNode *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return getHeight(node->left) - getHeight(node->right);
}

/*
    创建新节点
*/
AVLNode *createNode(int key)
{
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));

    if (node == NULL)
    {
        printf("节点内存分配失败\n");
        exit(1);
    }

    node->key = key;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/* =========================
   三、AVL 树旋转操作
   ========================= */

/*
    右旋

            y
           /
          x

    右旋后：

          x
           \
            y
*/
AVLNode *rotateRight(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // 执行旋转
    x->right = y;
    y->left = T2;

    // 先更新 y，再更新 x
    updateHeight(y);
    updateHeight(x);

    return x;
}

/*
    左旋

        x
         \
          y

    左旋后：

          y
         /
        x
*/
AVLNode *rotateLeft(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // 执行旋转
    y->left = x;
    x->right = T2;

    // 先更新 x，再更新 y
    updateHeight(x);
    updateHeight(y);

    return y;
}

/*
    平衡调整函数
*/
AVLNode *rebalance(AVLNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    // 先更新当前节点高度
    updateHeight(node);

    int balance = getBalanceFactor(node);

    /*
        LL 型失衡
        当前节点左边过高，并且左孩子的左边更高
        处理方式：右旋
    */
    if (balance > 1 && getBalanceFactor(node->left) >= 0)
    {
        return rotateRight(node);
    }

    /*
        LR 型失衡
        当前节点左边过高，但左孩子的右边更高
        处理方式：先对左孩子左旋，再对当前节点右旋
    */
    if (balance > 1 && getBalanceFactor(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    /*
        RR 型失衡
        当前节点右边过高，并且右孩子的右边更高
        处理方式：左旋
    */
    if (balance < -1 && getBalanceFactor(node->right) <= 0)
    {
        return rotateLeft(node);
    }

    /*
        RL 型失衡
        当前节点右边过高，但右孩子的左边更高
        处理方式：先对右孩子右旋，再对当前节点左旋
    */
    if (balance < -1 && getBalanceFactor(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

/* =========================
   四、AVL 树插入操作
   ========================= */

AVLNode *insertNode(AVLNode *node, int key, bool *inserted)
{
    /*
        第一步：按照二叉搜索树规则插入
    */
    if (node == NULL)
    {
        *inserted = true;
        return createNode(key);
    }

    if (key < node->key)
    {
        node->left = insertNode(node->left, key, inserted);
    }
    else if (key > node->key)
    {
        node->right = insertNode(node->right, key, inserted);
    }
    else
    {
        // 不允许插入重复值
        *inserted = false;
        return node;
    }

    /*
        第二步：插入完成后，回溯过程中调整平衡
    */
    return rebalance(node);
}

bool avlInsert(AVLTree *tree, int key)
{
    bool inserted = false;

    tree->root = insertNode(tree->root, key, &inserted);

    if (inserted)
    {
        tree->size++;
    }

    return inserted;
}

/* =========================
   五、AVL 树删除操作
   ========================= */

/*
    查找以 node 为根的子树中的最小节点
*/
AVLNode *findMinNode(AVLNode *node)
{
    while (node != NULL && node->left != NULL)
    {
        node = node->left;
    }

    return node;
}

AVLNode *deleteNode(AVLNode *node, int key, bool *deleted)
{
    if (node == NULL)
    {
        return NULL;
    }

    /*
        第一步：按照二叉搜索树规则查找要删除的节点
    */
    if (key < node->key)
    {
        node->left = deleteNode(node->left, key, deleted);
    }
    else if (key > node->key)
    {
        node->right = deleteNode(node->right, key, deleted);
    }
    else
    {
        /*
            找到要删除的节点
        */
        *deleted = true;

        /*
            情况 1：没有左孩子
            情况 2：没有右孩子
            这两种情况都可以用唯一的孩子替代当前节点
        */
        if (node->left == NULL || node->right == NULL)
        {
            AVLNode *child;

            if (node->left != NULL)
            {
                child = node->left;
            }
            else
            {
                child = node->right;
            }

            free(node);
            return child;
        }

        /*
            情况 3：左右孩子都存在

            使用右子树中的最小节点，即中序后继，
            替换当前节点的值。
        */
        AVLNode *successor = findMinNode(node->right);
        node->key = successor->key;

        /*
            删除右子树中的后继节点
            这里使用 dummy，避免重复修改 size
        */
        bool dummy = false;
        node->right = deleteNode(node->right, successor->key, &dummy);
    }

    /*
        第二步：删除完成后，回溯过程中调整平衡
    */
    return rebalance(node);
}

bool avlDelete(AVLTree *tree, int key)
{
    bool deleted = false;

    tree->root = deleteNode(tree->root, key, &deleted);

    if (deleted)
    {
        tree->size--;
    }

    return deleted;
}

/* =========================
   六、AVL 树查询管理函数
   ========================= */

bool avlSearch(AVLTree *tree, int key)
{
    AVLNode *current = tree->root;

    while (current != NULL)
    {
        if (key < current->key)
        {
            current = current->left;
        }
        else if (key > current->key)
        {
            current = current->right;
        }
        else
        {
            return true;
        }
    }

    return false;
}

int avlFindMin(AVLTree *tree)
{
    if (tree->root == NULL)
    {
        printf("树为空，无法获取最小值\n");
        exit(1);
    }

    AVLNode *node = findMinNode(tree->root);
    return node->key;
}

int avlFindMax(AVLTree *tree)
{
    if (tree->root == NULL)
    {
        printf("树为空，无法获取最大值\n");
        exit(1);
    }

    AVLNode *current = tree->root;

    while (current->right != NULL)
    {
        current = current->right;
    }

    return current->key;
}

int avlHeight(AVLTree *tree)
{
    return getHeight(tree->root);
}

int avlSize(AVLTree *tree)
{
    return tree->size;
}

bool avlIsEmpty(AVLTree *tree)
{
    return tree->root == NULL;
}

/* =========================
   七、非递归遍历
   ========================= */

/*
    非递归前序遍历
    顺序：根 -> 左 -> 右
*/
void preorderTraversal(AVLTree *tree)
{
    if (tree->root == NULL)
    {
        return;
    }

    Stack stack;
    initStack(&stack);

    push(&stack, tree->root);

    while (!isStackEmpty(&stack))
    {
        AVLNode *current = pop(&stack);

        printf("%d ", current->key);

        /*
            因为栈是后进先出，
            所以先压右孩子，再压左孩子。
            这样左孩子会先被访问。
        */
        if (current->right != NULL)
        {
            push(&stack, current->right);
        }

        if (current->left != NULL)
        {
            push(&stack, current->left);
        }
    }

    destroyStack(&stack);
}

/*
    非递归中序遍历
    顺序：左 -> 根 -> 右
*/
void inorderTraversal(AVLTree *tree)
{
    Stack stack;
    initStack(&stack);

    AVLNode *current = tree->root;

    while (current != NULL || !isStackEmpty(&stack))
    {
        /*
            第一步：一直向左走，
            并将沿途节点压入栈中。
        */
        while (current != NULL)
        {
            push(&stack, current);
            current = current->left;
        }

        /*
            第二步：左子树访问完毕，
            弹出栈顶节点并访问。
        */
        current = pop(&stack);
        printf("%d ", current->key);

        /*
            第三步：转向右子树。
        */
        current = current->right;
    }

    destroyStack(&stack);
}

/*
    非递归后序遍历
    顺序：左 -> 右 -> 根
*/
void postorderTraversal(AVLTree *tree)
{
    Stack stack;
    initStack(&stack);

    AVLNode *current = tree->root;
    AVLNode *lastVisited = NULL;

    while (current != NULL || !isStackEmpty(&stack))
    {
        /*
            第一步：一直向左走，
            并将沿途节点入栈。
        */
        while (current != NULL)
        {
            push(&stack, current);
            current = current->left;
        }

        /*
            第二步：查看栈顶节点，
            判断它的右子树是否已经访问。
        */
        AVLNode *topNode = peek(&stack);

        /*
            如果右子树存在，并且还没有访问过，
            就转向右子树。
        */
        if (topNode->right != NULL && lastVisited != topNode->right)
        {
            current = topNode->right;
        }
        else
        {
            /*
                如果右子树为空，
                或者右子树已经访问过，
                就可以访问当前根节点。
            */
            printf("%d ", topNode->key);
            lastVisited = topNode;
            pop(&stack);
        }
    }

    destroyStack(&stack);
}

/* =========================
   八、清空 AVL 树
   ========================= */

/*
    使用非递归方式释放所有节点
*/
void avlClear(AVLTree *tree)
{
    if (tree->root == NULL)
    {
        tree->size = 0;
        return;
    }

    Stack stack;
    initStack(&stack);

    push(&stack, tree->root);

    while (!isStackEmpty(&stack))
    {
        AVLNode *current = pop(&stack);

        if (current->left != NULL)
        {
            push(&stack, current->left);
        }

        if (current->right != NULL)
        {
            push(&stack, current->right);
        }

        free(current);
    }

    destroyStack(&stack);

    tree->root = NULL;
    tree->size = 0;
}

/* =========================
   九、AVL 树初始化
   ========================= */

void initAVLTree(AVLTree *tree)
{
    tree->root = NULL;
    tree->size = 0;
}

/* =========================
   十、测试代码
   ========================= */

int main()
{
    AVLTree tree;
    initAVLTree(&tree);

    int arr[] = {30, 20, 40, 10, 25, 35, 50, 5};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("插入数据：\n");
    for (int i = 0; i < n; i++)
    {
        avlInsert(&tree, arr[i]);
        printf("插入 %d，当前高度：%d\n", arr[i], avlHeight(&tree));
    }

    printf("\n前序遍历：");
    preorderTraversal(&tree);

    printf("\n中序遍历：");
    inorderTraversal(&tree);

    printf("\n后序遍历：");
    postorderTraversal(&tree);

    printf("\n\n节点数量：%d\n", avlSize(&tree));
    printf("树的高度：%d\n", avlHeight(&tree));
    printf("最小值：%d\n", avlFindMin(&tree));
    printf("最大值：%d\n", avlFindMax(&tree));

    int key = 25;
    printf("\n查找 %d：%s\n", key, avlSearch(&tree, key) ? "存在" : "不存在");

    printf("\n删除 20\n");
    avlDelete(&tree, 20);

    printf("删除后中序遍历：");
    inorderTraversal(&tree);

    printf("\n删除后高度：%d\n", avlHeight(&tree));
    printf("删除后节点数量：%d\n", avlSize(&tree));

    avlClear(&tree);

    return 0;
}