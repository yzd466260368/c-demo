#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* =========================
   一、霍夫曼树节点结构
   ========================= */

typedef struct HuffmanNode
{
    char ch;                   // 字符
    int weight;                // 权重，也可以理解为频率
    bool isLeaf;               // 是否为叶子节点
    struct HuffmanNode *left;  // 左孩子
    struct HuffmanNode *right; // 右孩子
} HuffmanNode;

/* =========================
   二、霍夫曼编码结构
   ========================= */

typedef struct HuffmanCode
{
    char ch;    // 字符
    char *code; // 对应的霍夫曼编码
} HuffmanCode;

/* =========================
   三、霍夫曼树管理结构
   ========================= */

typedef struct HuffmanTree
{
    HuffmanNode *root;  // 霍夫曼树根节点
    HuffmanCode *codes; // 编码表
    int codeCount;      // 编码数量
} HuffmanTree;

/* =========================
   四、最小堆结构
   ========================= */

typedef struct MinHeap
{
    HuffmanNode **data; // 存放霍夫曼节点指针
    int size;           // 当前堆中元素数量
    int capacity;       // 堆容量
} MinHeap;

/* =========================
   五、工具函数
   ========================= */

char *stringCopy(const char *src)
{
    char *dst = (char *)malloc(strlen(src) + 1);

    if (dst == NULL)
    {
        printf("字符串内存分配失败\n");
        exit(1);
    }

    strcpy(dst, src);
    return dst;
}

HuffmanNode *createHuffmanNode(char ch, int weight, bool isLeaf)
{
    HuffmanNode *node = (HuffmanNode *)malloc(sizeof(HuffmanNode));

    if (node == NULL)
    {
        printf("霍夫曼节点内存分配失败\n");
        exit(1);
    }

    node->ch = ch;
    node->weight = weight;
    node->isLeaf = isLeaf;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/* =========================
   六、最小堆管理函数
   ========================= */

void initMinHeap(MinHeap *heap, int capacity)
{
    heap->data = (HuffmanNode **)malloc(sizeof(HuffmanNode *) * capacity);

    if (heap->data == NULL)
    {
        printf("最小堆内存分配失败\n");
        exit(1);
    }

    heap->size = 0;
    heap->capacity = capacity;
}

void swapNode(HuffmanNode **a, HuffmanNode **b)
{
    HuffmanNode *temp = *a;
    *a = *b;
    *b = temp;
}

/*
    向上调整最小堆

    当新节点插入到堆尾后，
    如果它的权重比父节点小，
    就不断向上交换。
*/
void heapifyUp(MinHeap *heap, int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (heap->data[parent]->weight <= heap->data[index]->weight)
        {
            break;
        }

        swapNode(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

/*
    向下调整最小堆

    当堆顶元素被删除后，
    需要把最后一个元素放到堆顶，
    然后向下调整。
*/
void heapifyDown(MinHeap *heap, int index)
{
    while (1)
    {
        int smallest = index;
        int left = index * 2 + 1;
        int right = index * 2 + 2;

        if (left < heap->size &&
            heap->data[left]->weight < heap->data[smallest]->weight)
        {
            smallest = left;
        }

        if (right < heap->size &&
            heap->data[right]->weight < heap->data[smallest]->weight)
        {
            smallest = right;
        }

        if (smallest == index)
        {
            break;
        }

        swapNode(&heap->data[index], &heap->data[smallest]);
        index = smallest;
    }
}

void pushMinHeap(MinHeap *heap, HuffmanNode *node)
{
    if (heap->size >= heap->capacity)
    {
        heap->capacity *= 2;
        heap->data = (HuffmanNode **)realloc(
            heap->data,
            sizeof(HuffmanNode *) * heap->capacity);

        if (heap->data == NULL)
        {
            printf("最小堆扩容失败\n");
            exit(1);
        }
    }

    heap->data[heap->size] = node;
    heapifyUp(heap, heap->size);
    heap->size++;
}

HuffmanNode *popMinHeap(MinHeap *heap)
{
    if (heap->size == 0)
    {
        return NULL;
    }

    HuffmanNode *minNode = heap->data[0];

    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;

    heapifyDown(heap, 0);

    return minNode;
}

void destroyMinHeap(MinHeap *heap)
{
    free(heap->data);
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
}

/* =========================
   七、霍夫曼树初始化
   ========================= */

void initHuffmanTree(HuffmanTree *tree)
{
    tree->root = NULL;
    tree->codes = NULL;
    tree->codeCount = 0;
}

/* =========================
   八、生成霍夫曼编码
   ========================= */

/*
    递归生成霍夫曼编码

    规则：
    - 向左走，编码追加 0
    - 向右走，编码追加 1
    - 到达叶子节点时，得到一个完整编码
*/
void generateCodes(
    HuffmanTree *tree,
    HuffmanNode *node,
    char *buffer,
    int depth)
{
    if (node == NULL)
    {
        return;
    }

    /*
        如果是叶子节点，说明找到了一个字符的编码
    */
    if (node->isLeaf)
    {
        /*
            特殊情况：
            如果整棵树只有一个节点，
            那么给它分配编码 "0"。
        */
        if (depth == 0)
        {
            buffer[depth++] = '0';
        }

        buffer[depth] = '\0';

        tree->codes[tree->codeCount].ch = node->ch;
        tree->codes[tree->codeCount].code = stringCopy(buffer);
        tree->codeCount++;

        return;
    }

    /*
        访问左子树，追加 0
    */
    buffer[depth] = '0';
    generateCodes(tree, node->left, buffer, depth + 1);

    /*
        访问右子树，追加 1
    */
    buffer[depth] = '1';
    generateCodes(tree, node->right, buffer, depth + 1);
}

/* =========================
   九、构建霍夫曼树
   ========================= */

/*
    symbols：字符数组
    weights：权重数组
    n：字符数量
*/
bool buildHuffmanTree(
    HuffmanTree *tree,
    const char symbols[],
    const int weights[],
    int n)
{
    if (n <= 0)
    {
        return false;
    }

    MinHeap heap;
    initMinHeap(&heap, n * 2);

    /*
        第一步：
        将每个字符都创建成一个叶子节点，
        并放入最小堆。
    */
    for (int i = 0; i < n; i++)
    {
        HuffmanNode *node = createHuffmanNode(symbols[i], weights[i], true);
        pushMinHeap(&heap, node);
    }

    /*
        第二步：
        每次从最小堆中取出两个权重最小的节点，
        合并成一个新的父节点。
    */
    while (heap.size > 1)
    {
        HuffmanNode *left = popMinHeap(&heap);
        HuffmanNode *right = popMinHeap(&heap);

        HuffmanNode *parent = createHuffmanNode(
            '\0',
            left->weight + right->weight,
            false);

        parent->left = left;
        parent->right = right;

        /*
            第三步：
            将新生成的父节点重新放入最小堆。
        */
        pushMinHeap(&heap, parent);
    }

    /*
        最后堆中只剩一个节点，
        它就是霍夫曼树的根节点。
    */
    tree->root = popMinHeap(&heap);

    destroyMinHeap(&heap);

    /*
        第四步：
        根据霍夫曼树生成编码表。
    */
    tree->codes = (HuffmanCode *)malloc(sizeof(HuffmanCode) * n);

    if (tree->codes == NULL)
    {
        printf("编码表内存分配失败\n");
        exit(1);
    }

    tree->codeCount = 0;

    /*
        霍夫曼编码最长不会超过 n - 1。
        这里多分配几个空间，防止只有一个节点等特殊情况。
    */
    char *buffer = (char *)malloc(sizeof(char) * (n + 2));

    if (buffer == NULL)
    {
        printf("编码缓冲区内存分配失败\n");
        exit(1);
    }

    generateCodes(tree, tree->root, buffer, 0);

    free(buffer);

    return true;
}

/* =========================
   十、查找字符对应的编码
   ========================= */

const char *findCode(HuffmanTree *tree, char ch)
{
    for (int i = 0; i < tree->codeCount; i++)
    {
        if (tree->codes[i].ch == ch)
        {
            return tree->codes[i].code;
        }
    }

    return NULL;
}

/* =========================
   十一、打印霍夫曼编码表
   ========================= */

void printHuffmanCodes(HuffmanTree *tree)
{
    printf("霍夫曼编码表：\n");

    for (int i = 0; i < tree->codeCount; i++)
    {
        printf("字符 %c : %s\n", tree->codes[i].ch, tree->codes[i].code);
    }
}

/* =========================
   十二、字符串编码
   ========================= */

/*
    将普通字符串转换成霍夫曼编码字符串。

    例如：
    输入："ABCD"
    输出："010111..."
*/
char *huffmanEncode(HuffmanTree *tree, const char *text)
{
    int totalLength = 0;

    /*
        第一步：
        先计算编码后的总长度。
    */
    for (int i = 0; text[i] != '\0'; i++)
    {
        const char *code = findCode(tree, text[i]);

        if (code == NULL)
        {
            printf("字符 %c 不在霍夫曼编码表中\n", text[i]);
            return NULL;
        }

        totalLength += strlen(code);
    }

    /*
        第二步：
        分配结果字符串空间。
    */
    char *result = (char *)malloc(totalLength + 1);

    if (result == NULL)
    {
        printf("编码结果内存分配失败\n");
        exit(1);
    }

    /*
        第三步：
        将每个字符对应的编码拼接起来。
    */
    int pos = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        const char *code = findCode(tree, text[i]);
        int len = strlen(code);

        memcpy(result + pos, code, len);
        pos += len;
    }

    result[pos] = '\0';

    return result;
}

/* =========================
   十三、字符串解码
   ========================= */

/*
    将霍夫曼编码字符串还原成普通字符串。

    例如：
    输入："010111..."
    输出："ABCD"
*/
char *huffmanDecode(HuffmanTree *tree, const char *bits)
{
    if (tree->root == NULL)
    {
        return NULL;
    }

    int len = strlen(bits);

    /*
        解码后的字符数最多不会超过 bits 的长度。
    */
    char *result = (char *)malloc(len + 1);

    if (result == NULL)
    {
        printf("解码结果内存分配失败\n");
        exit(1);
    }

    int pos = 0;

    /*
        特殊情况：
        如果霍夫曼树只有一个叶子节点，
        那么每个 0 都表示该字符。
    */
    if (tree->root->isLeaf)
    {
        for (int i = 0; bits[i] != '\0'; i++)
        {
            if (bits[i] != '0')
            {
                printf("非法编码：单节点霍夫曼树只能使用 0\n");
                free(result);
                return NULL;
            }

            result[pos++] = tree->root->ch;
        }

        result[pos] = '\0';
        return result;
    }

    HuffmanNode *current = tree->root;

    /*
        从根节点开始：
        - 遇到 0，走左孩子
        - 遇到 1，走右孩子
        - 到达叶子节点，输出字符，并回到根节点
    */
    for (int i = 0; bits[i] != '\0'; i++)
    {
        if (bits[i] == '0')
        {
            current = current->left;
        }
        else if (bits[i] == '1')
        {
            current = current->right;
        }
        else
        {
            printf("非法编码字符：%c\n", bits[i]);
            free(result);
            return NULL;
        }

        if (current == NULL)
        {
            printf("非法霍夫曼编码\n");
            free(result);
            return NULL;
        }

        if (current->isLeaf)
        {
            result[pos++] = current->ch;
            current = tree->root;
        }
    }

    /*
        如果最后没有回到根节点，
        说明编码串是不完整的。
    */
    if (current != tree->root)
    {
        printf("编码串不完整\n");
        free(result);
        return NULL;
    }

    result[pos] = '\0';

    return result;
}

/* =========================
   十四、释放霍夫曼树
   ========================= */

void destroyHuffmanNode(HuffmanNode *node)
{
    if (node == NULL)
    {
        return;
    }

    destroyHuffmanNode(node->left);
    destroyHuffmanNode(node->right);

    free(node);
}

void destroyHuffmanTree(HuffmanTree *tree)
{
    destroyHuffmanNode(tree->root);

    for (int i = 0; i < tree->codeCount; i++)
    {
        free(tree->codes[i].code);
    }

    free(tree->codes);

    tree->root = NULL;
    tree->codes = NULL;
    tree->codeCount = 0;
}

/* =========================
   十五、测试代码
   ========================= */

int main()
{
    /*
        假设有 6 个字符，它们的出现频率如下：
        A:45, B:13, C:12, D:16, E:9, F:5
    */
    char symbols[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    int weights[] = {45, 13, 12, 16, 9, 5};
    int n = sizeof(symbols) / sizeof(symbols[0]);

    HuffmanTree tree;
    initHuffmanTree(&tree);

    if (!buildHuffmanTree(&tree, symbols, weights, n))
    {
        printf("霍夫曼树构建失败\n");
        return 1;
    }

    printHuffmanCodes(&tree);

    const char *text = "FACE";

    printf("\n原始字符串：%s\n", text);

    char *encoded = huffmanEncode(&tree, text);

    if (encoded != NULL)
    {
        printf("编码结果：%s\n", encoded);

        char *decoded = huffmanDecode(&tree, encoded);

        if (decoded != NULL)
        {
            printf("解码结果：%s\n", decoded);
            free(decoded);
        }

        free(encoded);
    }

    destroyHuffmanTree(&tree);

    return 0;
}