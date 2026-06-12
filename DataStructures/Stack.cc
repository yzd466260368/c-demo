#include <stdio.h>
#include <stdlib.h>

#define CAPACITY 10
#define RESIZE 10

typedef struct
{
    int *data;
    int top;      // 当前元素个数，也是下一个插入位置
    int capacity; // 容量
} Stack;

Stack *Init();
void ReleaseStack(Stack *stack);
int Resize(Stack *stack);
void push(Stack *stack, int value);
int pop(Stack *stack);
int peek(Stack *stack);
int is_stack_empty(Stack *stack);

int main(int argc, char const *argv[])
{
    Stack *stack = Init();

    push(stack, 10);
    push(stack, 20);
    push(stack, 30);

    printf("peek = %d\n", peek(stack));
    printf("pop = %d\n", pop(stack));
    printf("pop = %d\n", pop(stack));

    ReleaseStack(stack);

    return 0;
}

Stack *Init()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL)
    {
        return NULL;
    }

    stack->data = (int *)malloc(CAPACITY * sizeof(int));
    if (stack->data == NULL)
    {
        free(stack);
        return NULL;
    }

    stack->top = 0;
    stack->capacity = CAPACITY;

    return stack;
}

int Resize(Stack *stack)
{
    if (stack == NULL)
    {
        return 0;
    }

    int new_capacity = stack->capacity + RESIZE;

    int *new_data = (int *)realloc(stack->data, new_capacity * sizeof(int));
    if (new_data == NULL)
    {
        return 0;
    }

    stack->data = new_data;
    stack->capacity = new_capacity;

    return 1;
}

void ReleaseStack(Stack *stack)
{
    if (stack == NULL)
    {
        return;
    }

    free(stack->data);
    free(stack);
}

void push(Stack *stack, int value)
{
    if (stack == NULL)
    {
        return;
    }

    if (stack->top == stack->capacity)
    {
        if (!Resize(stack))
        {
            printf("扩容失败\n");
            return;
        }
    }

    stack->data[stack->top++] = value;
}

int pop(Stack *stack)
{
    if (stack == NULL || is_stack_empty(stack))
    {
        printf("栈为空，无法 pop\n");
        exit(EXIT_FAILURE);
    }

    return stack->data[--stack->top];
}

int peek(Stack *stack)
{
    if (stack == NULL || is_stack_empty(stack))
    {
        printf("栈为空，无法 peek\n");
        exit(EXIT_FAILURE);
    }

    return stack->data[stack->top - 1];
}

int is_stack_empty(Stack *stack)
{
    return stack == NULL || stack->top == 0;
}

// #include <stdio.h>
// #include <stdlib.h>

// #define CAPACITY 10
// #define RESIZE 10

// typedef struct
// {
//     int *data;
//     int top;      // 栈顶下标，也可以表示元素个数
//     int capacity; // 容量
// } Stack;

// Stack *Init();
// void ReleaseStack(Stack *stack);
// void push(Stack *s, int value);
// int pop(Stack *s);
// int peek(Stack *s);
// int is_stack_empty(Stack *s);

// int main(int argc, char const *argv[])
// {

//     return 0;
// }

// /*implement*/
// Stack *Init()
// {
//     Stack *stack = (Stack *)malloc(sizeof(Stack));
//     if (stack == NULL)
//     {
//         return NULL;
//     }
//     int *data = (int *)malloc(CAPACITY * sizeof(int));
//     if (data == NULL)
//     {
//         free(stack);
//         return NULL;
//     }
//     stack->data = data;
//     stack->top = 1;
//     stack->capacity = CAPACITY;
//     return stack;
// }

// Stack *Resize(Stack *stack)
// {
//     int new_size = stack->capacity + RESIZE;
//     int *data = realloc(stack->data, new_size * sizeof(int));
//     if (data == NULL)
//     {
//         // 扩容失败
//         return NULL;
//     }
//     stack->data = data;
//     stack->capacity = new_size;
//     return stack;
// }

// void ReleaseStack(Stack *stack)
// {
//     if (stack == NULL)
//     {
//         return;
//     }
//     free(stack->data);
//     free(stack);
// }

// void push(Stack *stack, int value)
// {
//     if (stack == NULL)
//     {
//         printf("压栈失败\n");
//         return;
//     }
//     if (stack->top == stack->capacity)
//     {
//         stack = Resize(stack);
//         if (stack == NULL)
//         {
//             printf("扩容失败\n");
//             return;
//         }
//     }
//     stack->data[stack->top - 1] = value;
//     // *(stack->data + stack->top - 1) = value;
//     stack->top = stack->top + 1;
// }
