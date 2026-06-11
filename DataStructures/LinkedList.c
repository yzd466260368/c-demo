#include <stdio.h>
#include <stdlib.h>

typedef int DataType;

typedef struct Node
{
    DataType data;
    struct Node *next;
} Node;

typedef struct LinkedList
{
    Node *head;
    Node *tail;
    size_t size;
} LL;

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
// void ReverseList(LL *ll)
// {
//     if (ll->head == NULL || ll->head->next == NULL)
//     {
//         return;
//     }
//     Node *tmp = ll->head->next->next;
//     ll->head->next->next = NULL;

//     Node *p = NULL;
//     while (tmp != NULL)
//     {
//         p = tmp;
//         tmp = tmp->next;

//         p->next = ll->head->next;
//         ll->head->next = p;
//     }
// }
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

int main(int argc, char const *argv[])
{
    LL *ll = InitLinkedList();
    for (int i = 0; i < 100; i++)
    {
        AddLast(ll, i);
    }
    Show(ll);
    printf("链表的长度是：%lld\n", GetLinkedListSize(ll));

    ReverseLinkedList(ll);
    Show(ll);
    printf("链表的长度是：%lld\n", GetLinkedListSize(ll));

    return 0;
}
