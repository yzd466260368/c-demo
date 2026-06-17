#include <stdio.h>

struct TreeNode
{
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

int main(int argc, char const *argv[])
{
    struct TreeNode node1 = {1, NULL, NULL};
    struct TreeNode node2 = {2, NULL, NULL};
    struct TreeNode node3 = {3, NULL, NULL};
    struct TreeNode root = {4, NULL, NULL};
    struct TreeNode node5 = {5, NULL, NULL};
    node2.left = &node1;
    node2.right = &node3;
    root.left = &node2;
    root.right = &node5;

    // 用数组模拟栈，存放 TreeNode 指针
    struct TreeNode *stack[100];

    // top 表示栈顶位置，-1 表示栈为空
    int top = -1;

    // 先把根节点入栈
    stack[++top] = &root;

    printf("非递归前序遍历结果：");

    while (top != -1)
    {
        // 取出栈顶节点
        struct TreeNode *cur = stack[top--];

        // 前序遍历：先访问根节点
        printf("%d ", cur->val);

        /*
         * 因为栈是后进先出，
         * 所以要先压右孩子，再压左孩子。
         *
         * 这样下一次出栈的才会是左孩子。
         */
        if (cur->right != NULL)
        {
            stack[++top] = cur->right;
        }

        if (cur->left != NULL)
        {
            stack[++top] = cur->left;
        }
    }

    return 0;
}

int depth(struct TreeNode *root)
{
    if (root == NULL)
    {
        return 0;
    }
    int leftDepth = depth(root->left);
    int rightDepth = depth(root->right);

    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

void front(struct TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    printf("%d ", root->val);
    front(root->left);
    front(root->right);
}
