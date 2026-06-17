#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/*
    双向循环链表节点
*/
typedef struct DNode
{
    int data;
    struct DNode *prev;
    struct DNode *next;
} DNode;

/*
    双向循环链表本体

    head 是哨兵头结点，不存有效数据。
    size 表示有效节点数量。
*/
typedef struct
{
    DNode *head;
    size_t size;
} DList;

/*
    函数声明
*/
DList *dlist_create(void);
void dlist_destroy(DList *list);
void dlist_clear(DList *list);

int dlist_is_empty(const DList *list);
size_t dlist_size(const DList *list);

int dlist_push_front(DList *list, int value);
int dlist_push_back(DList *list, int value);

int dlist_pop_front(DList *list, int *out_value);
int dlist_pop_back(DList *list, int *out_value);

int dlist_insert(DList *list, size_t index, int value);
int dlist_remove_at(DList *list, size_t index, int *out_value);
int dlist_remove_value(DList *list, int value);

DNode *dlist_find(DList *list, int value);

void dlist_print_forward(const DList *list);
void dlist_print_backward(const DList *list);

/*
    内部辅助函数：创建节点
*/
static DNode *dnode_create(int value)
{
    DNode *node = malloc(sizeof(DNode));

    if (node == NULL)
    {
        return NULL;
    }

    node->data = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

/*
    内部辅助函数：把 node 插入到 prev 和 next 中间

    插入前：
        prev <-> next

    插入后：
        prev <-> node <-> next
*/
static void dlist_link_between(DNode *prev, DNode *next, DNode *node)
{
    node->prev = prev;
    node->next = next;

    prev->next = node;
    next->prev = node;
}

/*
    内部辅助函数：把 node 从链表中断开

    删除前：
        prev <-> node <-> next

    删除后：
        prev <-> next
*/
static void dlist_unlink(DNode *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;

    node->prev = NULL;
    node->next = NULL;
}

/*
    内部辅助函数：根据下标获取节点

    index 从 0 开始。

    例如：
        index = 0 表示第一个有效节点
        index = size - 1 表示最后一个有效节点
*/
static DNode *dlist_get_node(const DList *list, size_t index)
{
    if (list == NULL || index >= list->size)
    {
        return NULL;
    }

    DNode *cur = NULL;

    /*
        优化：
        如果 index 在前半部分，就从前往后找。
        如果 index 在后半部分，就从后往前找。
    */
    if (index < list->size / 2)
    {
        cur = list->head->next;

        for (size_t i = 0; i < index; i++)
        {
            cur = cur->next;
        }
    }
    else
    {
        cur = list->head->prev;

        for (size_t i = list->size - 1; i > index; i--)
        {
            cur = cur->prev;
        }
    }

    return cur;
}

/*
    创建双向循环链表
*/
DList *dlist_create(void)
{
    DList *list = malloc(sizeof(DList));

    if (list == NULL)
    {
        return NULL;
    }

    DNode *head = malloc(sizeof(DNode));

    if (head == NULL)
    {
        free(list);
        return NULL;
    }

    /*
        哨兵头结点不存有效数据。

        空链表时：
            head->next 指向自己
            head->prev 指向自己
    */
    head->data = 0;
    head->next = head;
    head->prev = head;

    list->head = head;
    list->size = 0;

    return list;
}

/*
    清空链表中的所有有效节点，但保留 head
*/
void dlist_clear(DList *list)
{
    if (list == NULL || list->head == NULL)
    {
        return;
    }

    DNode *cur = list->head->next;

    /*
        因为是循环链表，所以不能用 cur != NULL 判断结束。
        应该判断 cur 是否又回到了 head。
    */
    while (cur != list->head)
    {
        DNode *next = cur->next;
        free(cur);
        cur = next;
    }

    /*
        清空后重新恢复为空链表状态
    */
    list->head->next = list->head;
    list->head->prev = list->head;
    list->size = 0;
}

/*
    销毁整个链表
*/
void dlist_destroy(DList *list)
{
    if (list == NULL)
    {
        return;
    }

    dlist_clear(list);

    free(list->head);
    free(list);
}

/*
    判断链表是否为空
*/
int dlist_is_empty(const DList *list)
{
    return list == NULL || list->size == 0;
}

/*
    返回链表长度
*/
size_t dlist_size(const DList *list)
{
    if (list == NULL)
    {
        return 0;
    }

    return list->size;
}

/*
    头插法

    插入前：
        head <-> old_first

    插入后：
        head <-> new_node <-> old_first
*/
int dlist_push_front(DList *list, int value)
{
    if (list == NULL || list->head == NULL)
    {
        return 0;
    }

    DNode *node = dnode_create(value);

    if (node == NULL)
    {
        return 0;
    }

    dlist_link_between(list->head, list->head->next, node);
    list->size++;

    return 1;
}

/*
    尾插法

    插入前：
        old_last <-> head

    插入后：
        old_last <-> new_node <-> head
*/
int dlist_push_back(DList *list, int value)
{
    if (list == NULL || list->head == NULL)
    {
        return 0;
    }

    DNode *node = dnode_create(value);

    if (node == NULL)
    {
        return 0;
    }

    dlist_link_between(list->head->prev, list->head, node);
    list->size++;

    return 1;
}

/*
    头删

    删除第一个有效节点。
*/
int dlist_pop_front(DList *list, int *out_value)
{
    if (dlist_is_empty(list))
    {
        return 0;
    }

    DNode *node = list->head->next;

    if (out_value != NULL)
    {
        *out_value = node->data;
    }

    dlist_unlink(node);
    free(node);
    list->size--;

    return 1;
}

/*
    尾删

    删除最后一个有效节点。
*/
int dlist_pop_back(DList *list, int *out_value)
{
    if (dlist_is_empty(list))
    {
        return 0;
    }

    DNode *node = list->head->prev;

    if (out_value != NULL)
    {
        *out_value = node->data;
    }

    dlist_unlink(node);
    free(node);
    list->size--;

    return 1;
}

/*
    在指定位置插入元素

    index = 0       表示插入到头部
    index = size    表示插入到尾部
    index > size    非法
*/
int dlist_insert(DList *list, size_t index, int value)
{
    if (list == NULL || list->head == NULL)
    {
        return 0;
    }

    if (index > list->size)
    {
        return 0;
    }

    DNode *node = dnode_create(value);

    if (node == NULL)
    {
        return 0;
    }

    /*
        找到要插入位置的后一个节点 target。

        插入逻辑：
            target->prev <-> node <-> target
    */
    DNode *target = NULL;

    if (index == list->size)
    {
        target = list->head;
    }
    else
    {
        target = dlist_get_node(list, index);
    }

    dlist_link_between(target->prev, target, node);
    list->size++;

    return 1;
}

/*
    删除指定下标的节点

    index = 0 表示删除第一个有效节点。
*/
int dlist_remove_at(DList *list, size_t index, int *out_value)
{
    if (dlist_is_empty(list))
    {
        return 0;
    }

    if (index >= list->size)
    {
        return 0;
    }

    DNode *node = dlist_get_node(list, index);

    if (node == NULL)
    {
        return 0;
    }

    if (out_value != NULL)
    {
        *out_value = node->data;
    }

    dlist_unlink(node);
    free(node);
    list->size--;

    return 1;
}

/*
    查找第一个值等于 value 的节点

    找到返回节点指针。
    找不到返回 NULL。
*/
DNode *dlist_find(DList *list, int value)
{
    if (list == NULL || list->head == NULL)
    {
        return NULL;
    }

    DNode *cur = list->head->next;

    while (cur != list->head)
    {
        if (cur->data == value)
        {
            return cur;
        }

        cur = cur->next;
    }

    return NULL;
}

/*
    删除第一个值等于 value 的节点
*/
int dlist_remove_value(DList *list, int value)
{
    if (dlist_is_empty(list))
    {
        return 0;
    }

    DNode *node = dlist_find(list, value);

    if (node == NULL)
    {
        return 0;
    }

    dlist_unlink(node);
    free(node);
    list->size--;

    return 1;
}

/*
    从头到尾打印
*/
void dlist_print_forward(const DList *list)
{
    if (list == NULL || list->head == NULL)
    {
        return;
    }

    DNode *cur = list->head->next;

    printf("正向: ");

    while (cur != list->head)
    {
        printf("%d ", cur->data);
        cur = cur->next;
    }

    printf("\n");
}

/*
    从尾到头打印
*/
void dlist_print_backward(const DList *list)
{
    if (list == NULL || list->head == NULL)
    {
        return;
    }

    DNode *cur = list->head->prev;

    printf("反向: ");

    while (cur != list->head)
    {
        printf("%d ", cur->data);
        cur = cur->prev;
    }

    printf("\n");
}

/*
    测试代码
*/
int main(void)
{
    DList *list = dlist_create();

    if (list == NULL)
    {
        printf("链表创建失败\n");
        return 1;
    }

    dlist_push_back(list, 10);
    dlist_push_back(list, 20);
    dlist_push_back(list, 30);

    dlist_push_front(list, 5);

    dlist_print_forward(list);
    dlist_print_backward(list);

    printf("size = %zu\n", dlist_size(list));

    dlist_insert(list, 2, 15);
    dlist_print_forward(list);

    int value;

    if (dlist_pop_front(list, &value))
    {
        printf("头删元素: %d\n", value);
    }

    if (dlist_pop_back(list, &value))
    {
        printf("尾删元素: %d\n", value);
    }

    dlist_print_forward(list);

    if (dlist_remove_at(list, 1, &value))
    {
        printf("删除下标 1 的元素: %d\n", value);
    }

    dlist_print_forward(list);

    dlist_push_back(list, 100);
    dlist_push_back(list, 200);
    dlist_push_back(list, 300);

    dlist_print_forward(list);

    if (dlist_remove_value(list, 200))
    {
        printf("删除值 200 成功\n");
    }

    dlist_print_forward(list);
    dlist_print_backward(list);

    dlist_destroy(list);

    return 0;
}