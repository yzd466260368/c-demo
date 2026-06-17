#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef int DataType;

typedef struct Node
{
    DataType data;
    struct Node *next;
} Node;

typedef struct LinkedList
{
    Node *head;  // 头节点
    Node *tail;  // 尾节点
    size_t size; // 当前已有元素个数
} LL;

typedef struct ArrayList
{

    DataType *data;
    size_t count;    // 当前已有元素个数
    size_t capacity; // 当前容量
} AL;

Node *CreateNode(int value)
{
    Node *new_node = (Node *)calloc(1, sizeof(Node));
    if (new_node == NULL)
    {
        return NULL;
    }

    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}

LL *InitLinkedList()
{
    LL *ll = (LL *)calloc(1, sizeof(LL));
    if (ll == NULL)
    {
        return NULL;
    }
    ll->head = NULL;
    ll->tail = NULL;
    ll->size = 0;
    return ll;
}

void FreeList(LL *ll)
{
    if (ll == NULL)
    {
        return;
    }

    Node *cur = ll->head;
    Node *next = NULL;

    // 1. 遍历释放每一个节点
    while (cur != NULL)
    {
        next = cur->next; // 先保存下一个节点
        free(cur);        // 释放当前节点
        cur = next;
    }

    // 2. 最后释放链表管理结构体
    free(ll);
}

size_t GetLinkedListSize(LL *ll)
{
    if (ll == NULL)
    {
        return -1;
    }
    return ll->size;
}

int AddLast(LL *ll, DataType value)
{
    if (ll == NULL)
    {
        return -1;
    }
    Node *new_node = CreateNode(value);
    if (new_node == NULL)
    {
        return -1;
    }
    if (ll->size == 0)
    {
        ll->head = new_node;
        ll->tail = new_node;
    }
    else
    {
        ll->tail->next = new_node;
        ll->tail = new_node;
    }

    ll->size++;
    return 0;
}

int AddFirst(LL *ll, DataType value)
{
    if (ll == NULL)
    {
        return -1;
    }
    Node *new_node = CreateNode(value);
    if (new_node == NULL)
    {
        return -1;
    }
    if (ll->size == 0)
    {
        ll->head = new_node;
        ll->tail = new_node;
    }
    else
    {
        new_node->next = ll->head;
        ll->head = new_node;
    }

    ll->size++;
    return 0;
}

int findDataType(LL *ll, DataType value)
{
    if (ll == NULL)
    {
        return -1;
    }
    Node *node = ll->head;
    int index = 0;
}

void Show(LL *ll)
{
    if (ll == NULL)
    {
        return;
    }
    Node *node = ll->head;
    while (node != NULL)
    {
        printf("%d ", node->data);
        node = node->next;
    }
    putchar('\n');
}

// 反转链表
void ReverseLinkedList(LL *ll)
{
    if (ll == NULL || ll->size <= 1)
    {
        return;
    }
    Node *prev = NULL;
    Node *cur = ll->head;
    Node *next = NULL;

    ll->tail = ll->head;

    while (cur != NULL)
    {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }

    ll->head = prev;
}
// 排序
int SortMin2Max(LL *ll)
{
    if (ll == NULL || ll->size == 0)
    {
        return -1;
    }
    Node *min_node = NULL;
    Node *cur = ll->head;

    while (cur != NULL)
    {
        Node *next = cur->next;

        if (min_node == NULL || cur->data <= min_node->data)
        {
            cur->next = min_node;
            min_node = cur;
        }
        else
        {
            Node *p = min_node;
            while (p->next != NULL && p->next->data <= cur->data)
            {
                p = p->next;
            }
            // p->next->data > cur->data
            cur->next = p->next;
            p->next = cur;
        }

        cur = next;
    }

    ll->head = min_node;

    Node *tail = ll->head;
    while (tail->next != NULL)
    {
        tail = tail->next;
    }

    ll->tail = tail;

    return 0;
}

// int SortMax2Min(LL *ll)
// {
//     if (ll == NULL || ll->size <= 1)
//     {
//         return 0;
//     }

//     Node *max_head = NULL;
//     Node *cur = ll->head;

//     while (cur != NULL)
//     {
//         Node *next = cur->next;

//         if (max_head == NULL || cur->data >= max_head->data)
//         {
//             cur->next = max_head;
//             max_head = cur;
//         }
//         else
//         {
//             Node *p = max_head;

//             while (p->next != NULL && p->next->data >= cur->data)
//             {
//                 p = p->next;
//             }

//             cur->next = p->next;
//             p->next = cur;
//         }

//         cur = next;
//     }

//     ll->head = max_head;

//     Node *tail = ll->head;
//     while (tail->next != NULL)
//     {
//         tail = tail->next;
//     }

//     ll->tail = tail;

//     return 0;
// }

int SortMax2Min(LL *ll)
{
    if (ll == NULL || ll->size == 0)
    {
        return -1;
    }

    Node *max_head = NULL;
    Node *cur = ll->head;

    while (cur != NULL)
    {
        Node *next = cur->next;

        if (max_head == NULL || max_head->data <= cur->data)
        {
            cur->next = max_head;
            max_head = cur;
        }
        else
        {
            Node *p = max_head;
            while (p->next != NULL && p->next->data >= cur->data)
            {
                p = p->next;
            }

            cur->next = p->next;
            p->next = cur;
        }

        cur = next;
    }

    ll->head = max_head;

    Node *tail = ll->head;
    while (tail->next != NULL)
    {
        tail = tail->next;
    }
    ll->tail = tail;

    return 0;
}

// 判断链表是否成环
int IsLoop(LL *ll)
{
    // 快慢指针
    if (ll == NULL || ll->head == NULL)
    {
        return 0;
    }

    Node *quick = ll->head;
    Node *slow = ll->head;

    while (quick != NULL && quick->next != NULL)
    {
        quick = quick->next->next;
        slow = slow->next;
        if (quick == slow)
        {
            return 1;
        }
    }

    return 0;
}

// 合并有序链表
LL *Combine_List(LL *ll1, LL *ll2)
{
    if (ll1 == NULL)
        return ll2;
    if (ll2 == NULL)
        return ll1;

    Node *p1 = ll1->head;
    Node *p2 = ll2->head;

    Node dummy;
    Node *tail = &dummy;
    dummy.next = NULL;

    while (p1 != NULL && p2 != NULL)
    {
        if (p1->data <= p2->data)
        {
            tail->next = p1;
            p1 = p1->next;
        }
        else
        {
            tail->next = p2;
            p2 = p2->next;
        }

        tail = tail->next;
    }

    if (p1 != NULL)
    {
        tail->next = p1;
    }
    else
    {
        tail->next = p2;
    }

    while (tail->next != NULL)
    {
        tail = tail->next;
    }

    ll1->head = dummy.next;
    ll1->tail = tail;
    ll1->size = ll1->size + ll2->size;

    ll2->head = NULL;
    ll2->tail = NULL;
    free(ll2);

    return ll1;
}

#include <time.h>

int main(int argc, char const *argv[])
{
    LL *ll = InitLinkedList();

    srand((unsigned int)time(NULL)); // 只需要调用一次

    for (int i = 0; i < 100; i++)
    {

        int num = rand() % 100; // 生成 0 ~ 99 的随机数
        AddLast(ll, num);
    }
    Show(ll);
    printf("链表的长度是：%lld\n", GetLinkedListSize(ll));

    // ReverseLinkedList(ll);
    // Show(ll);
    // printf("链表的长度是：%lld\n", GetLinkedListSize(ll));
    SortMax2Min(ll);
    Show(ll);
    printf("链表的长度是：%lld\n", GetLinkedListSize(ll));

    SortMin2Max(ll);
    Show(ll);
    printf("链表的长度是：%lld\n", GetLinkedListSize(ll));

    return 0;
}

/*
// 反转链表
void ReverseList(LL *ll)
{
    if (ll->head == NULL || ll->head->next == NULL)
    {
        return;
    }
    Node *tmp = ll->head->next->next;
    ll->head->next->next = NULL;

    Node *p = NULL;
    while (tmp != NULL)
    {
        p = tmp;
        tmp = tmp->next;

        p->next = ll->head->next;
        ll->head->next = p;
    }
}
// 排序
int SortByValue(LL *ll)
{
    if (ll == NULL || ll->size <= 1)
    {
        return -1;
    }

    Node *i = ll->head;

    while (i != NULL)
    {
        Node *min = i;
        Node *j = i->next;

        while (j != NULL)
        {
            if (j->data < min->data)
            {
                min = j;
            }

            j = j->next;
        }

        if (min != i)
        {
            DataType temp = i->data;
            i->data = min->data;
            min->data = temp;
        }

        i = i->next;
    }

    return 0;
}

int SortByNode(LL *ll)
{
    if (ll == NULL || ll->size <= 1)
    {
        return -1;
    }

    Node *sorted_head = NULL;
    Node *cur = ll->head;

    while (cur != NULL)
    {
        Node *next = cur->next;

        if (sorted_head == NULL || cur->data < sorted_head->data)
        {
            cur->next = sorted_head;
            sorted_head = cur;
        }
        else
        {
            Node *p = sorted_head;

            while (p->next != NULL && p->next->data <= cur->data)
            {
                p = p->next;
            }

            cur->next = p->next;
            p->next = cur;
        }

        cur = next;
    }

    ll->head = sorted_head;

    Node *tail = ll->head;
    while (tail->next != NULL)
    {
        tail = tail->next;
    }

    ll->tail = tail;

    return 0;
}

int BubbleSortByValue(LL *ll)
{
    if (ll == NULL || ll->size <= 1)
    {
        return -1;
    }

    int swapped;

    do
    {
        swapped = 0;

        Node *cur = ll->head;

        while (cur != NULL && cur->next != NULL)
        {
            if (cur->data > cur->next->data)
            {
                DataType temp = cur->data;
                cur->data = cur->next->data;
                cur->next->data = temp;

                swapped = 1;
            }

            cur = cur->next;
        }

    } while (swapped);

    return 0;
}

int BubbleSortByNode(LL *ll)
{
    if (ll == NULL || ll->size <= 1)
    {
        return -1;
    }

    int swapped;

    do
    {
        swapped = 0;

        Node *prev = NULL;
        Node *cur = ll->head;

        while (cur != NULL && cur->next != NULL)
        {
            Node *next = cur->next;

            if (cur->data > next->data)
            {
                cur->next = next->next;
                next->next = cur;

                if (prev == NULL)
                {
                    ll->head = next;
                }
                else
                {
                    prev->next = next;
                }

                prev = next;
                swapped = 1;
            }
            else
            {
                prev = cur;
                cur = cur->next;
            }
        }

    } while (swapped);

    Node *tail = ll->head;
    while (tail->next != NULL)
    {
        tail = tail->next;
    }

    ll->tail = tail;

    return 0;
}
*/
