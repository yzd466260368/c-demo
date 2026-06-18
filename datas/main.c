#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int value;
    struct Node *next;
} Node;

void joseph(int n, int k, int m)
{
    if (n <= 0 || k < 1 || k > n || m <= 0)
    {
        return;
    }

    /*
    1. 初始化链表，head=tail构成循环链表
    2. 初始化node * pre=tail 和 cur=head
    3. 通过for循环找到k点的位置，此时cur为k点位置，pre为k-1的位置
    4. 开始出队处理，初始化count = n，当count > 0就执行循环，从cur开始，即k点位置，通过for循环找到m的位置，针对cur点做出队处理
    */

    // 初始化链表
    Node *head = NULL, *tail = NULL;
    for (int i = 1; i <= n; i++)
    {
        Node *p = (Node *)malloc(sizeof(Node));
        p->value = i;
        p->next = NULL;

        if (head == NULL)
        {
            head = p;
            tail = p;
        }
        else
        {
            tail->next = p;
            tail = p;
        }
    }

    tail->next = head; // 构成循环链表

    // 找到K的位置
    Node *pre = tail;
    Node *cur = head;

    for (int i = 1; i < k; i++)
    {
        pre = cur;
        cur = cur->next; // cur在第K的位置
    }

    printf("出队序列为：(");
    // 开始出队
    int count = n;

    while (count > 0)
    {
        // 从cur走到m的位置
        for (int i = 1; i < m; i++)
        {
            pre = cur;
            cur = cur->next;
        }

        // 此时cur为要出队的人
        printf("%d", cur->value);
        if (count > 1)
        {
            printf(", ");
        }

        // 删除cur
        pre->next = cur->next;
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);

        count--;
    }

    printf(")。\n");
}

int main(int argc, char const *argv[])
{
    /*
    使用无头单向循环链表编写约瑟夫环的出队函数：
    void joseph(int n, int k, int m);  //先初始化再依次出队
    设编号分别为：1，2，…，n的n个人围坐一圈。约定序号为k（1≤k≤n）的人从1开始计数，数到m的那个人出列，他的下一位又从1开始计数，数到m的那个人又出列，依次类推，直到所有人出列为止。
    例： 设n=8，k=3，m=4时，如图所示。
    出列序列为：(6，2，7，4，3，5，1，8)。
    */
    joseph(8, 3, 4);
    return 0;
}
