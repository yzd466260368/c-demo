#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define DataType int

typedef struct Node
{
    DataType val;
    struct Node *next;
} Node;

typedef struct LinkedList
{
    Node *head;
    size_t size;
} LinkedList;

Node *CreateNode(DataType val)
{

    Node *new_node = (Node *)calloc(1, sizeof(Node));
    if (new_node == NULL)
    {
        return NULL;
    }
    new_node->val = val;
    new_node->next = NULL;
    return new_node;
}

LinkedList *InitLinkedList()
{
    LinkedList *linkedList = (LinkedList *)calloc(1, sizeof(LinkedList));
    if (linkedList == NULL)
    {
        // 内存分配失败
        return NULL;
    }
    linkedList->head = NULL;
    linkedList->size = 0;
    return linkedList;
}

void AddFirst(LinkedList *linkedList, DataType val)
{
    if (linkedList == NULL)
    {
        return;
    }
    Node *new_node = CreateNode(val);
    if (new_node == NULL)
    {
        return;
    }
    new_node->next = linkedList->head;
    linkedList->head = new_node;
    linkedList->size++;
}

void PrintLinkedList(LinkedList *linkedList)
{
    if (linkedList == NULL)
        return;
    Node *cur = linkedList->head;
    while (cur != NULL)
    {
        printf("%d ", cur->val);
        cur = cur->next;
    }
    putchar('\n');
}

void DestroyLinkedList(LinkedList *linkedList)
{
    if (linkedList == NULL)
        return;

    Node *cur = linkedList->head;
    while (cur != NULL)
    {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }

    free(linkedList);
}

// 1.  删除链表中所有满足Node.val == val的节点，并返回新的头节点
Node *RemoveVal(Node *head, DataType val)
{
    // 使用虚拟头节点
    Node dummy;
    dummy.next = head;

    Node *cur = &dummy;

    while (cur->next != NULL)
    {
        if (cur->next->val == val)
        {
            Node *tmp = cur->next;
            cur->next = tmp->next;
            free(tmp);
        }
        else
        {
            cur = cur->next;
        }
    }
    return dummy.next;
}

// 2. 返回链表的中间节点，如果有两个中间节点，返回第二个中间节点
Node *ReturMidNode(Node *head)
{
    // 快慢指针
    if (head == NULL)
        return NULL;

    Node *slow = head;
    Node *fast = head;

    while (fast != NULL && fast->next != NULL) // 因为fast可能停在最后一个或者倒数第二个
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// 3. 返回链表中倒数第K个节点
Node *FindKthFromEnd(Node *head, size_t k)
{
    if (head == NULL || k == 0)
        return NULL;

    Node *slow = head;
    Node *fast = head;

    // 先让快指针走k步
    for (size_t i = 0; i < k; i++)
    {
        if (fast == NULL)
        {
            // k 大于链表长度
            return NULL;
        }
        fast = fast->next;
    }
    // 再同时走
    while (fast != NULL)
    {
        slow = slow->next;
        fast = fast->next;
    }
    return slow;
}

// 4. 反转链表后并返回反转后的链表
Node *ReverseList(Node *head)
{
    if (head == NULL)
    {
        return NULL;
    }

    Node *pre = NULL;
    Node *cur = head;
    Node *next = NULL;

    while (cur != NULL)
    {
        next = cur->next;
        cur->next = pre;
        pre = cur;
        cur = next;
    }

    return pre;
}

// 5. 访问链表中第K个元素，并打印出其值
void PrintK(Node *head, size_t k)
{
    if (head == NULL || k == 0)
    {
        printf("Invalid input\n");
        return;
    }

    Node *cur = head;

    for (size_t i = 1; i < k; i++)
    {
        if (cur == NULL)
        {
            printf("K out of range\n");
            return;
        }

        cur = cur->next;
    }

    if (cur == NULL)
    {
        printf("K out of range\n");
        return;
    }

    printf("%d\n", cur->val);
}

// 6. 请编写一个程序，在链表的第k个位置插入一个元素
void AddKth(Node *head, size_t k, DataType val)
{
    if (head == NULL)
    {
        return;
    }
    Node *cur = head;
    for (size_t i = 1; i < k - 1; i++)
    {
        if (cur == NULL)
        {
            return;
        }
        cur = cur->next;
    }
    if (cur == NULL)
    {
        return;
    }
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL)
    {
        return;
    }
    new_node->next = NULL;
    new_node->val = val;

    new_node->next = cur->next;
    cur->next = new_node;
}

int main(int argc, char const *argv[])
{
    LinkedList *linkedList = InitLinkedList();
    for (int i = 0; i < 10; i++)
    {
        AddFirst(linkedList, i + 1);
    }
    PrintLinkedList(linkedList);

    linkedList->head = RemoveVal(linkedList->head, 10);

    PrintLinkedList(linkedList);

    DestroyLinkedList(linkedList);
    return 0;
}
