#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE_SIZE 256
#define MAX_NODES 512
#define MAX_CODE_LEN 256

typedef struct Node
{
    unsigned char ch;
    int freq;
    struct Node *left;
    struct Node *right;
} Node;

/* 创建一个哈夫曼树节点 */
Node *createNode(unsigned char ch, int freq)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->ch = ch;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* 判断是否为叶子节点：没有左右孩子就是叶子 */
int isLeaf(Node *node)
{
    return node->left == NULL && node->right == NULL;
}

/* 释放哈夫曼树内存 */
void freeTree(Node *root)
{
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

/* 统计原文件中每个字节出现的频率 */
void countFrequency(FILE *in, int freq[])
{
    int ch;

    while ((ch = fgetc(in)) != EOF)
    {
        freq[(unsigned char)ch]++;
    }
}

/* 构建哈夫曼树 */
Node *buildHuffmanTree(int freq[])
{
    Node *nodes[MAX_NODES];
    int count = 0;

    /* 把出现过的字符都创建成叶子节点 */
    for (int i = 0; i < BYTE_SIZE; i++)
    {
        if (freq[i] > 0)
        {
            nodes[count++] = createNode((unsigned char)i, freq[i]);
        }
    }

    if (count == 0)
    {
        return NULL;
    }

    /* 特殊情况：文件中只有一种字符 */
    if (count == 1)
    {
        Node *root = createNode(0, nodes[0]->freq);
        root->left = nodes[0];
        return root;
    }

    /* 每次找出频率最小的两个节点，合并成一个新节点 */
    while (count > 1)
    {
        int min1 = -1;
        int min2 = -1;

        for (int i = 0; i < count; i++)
        {
            if (min1 == -1 || nodes[i]->freq < nodes[min1]->freq)
            {
                min2 = min1;
                min1 = i;
            }
            else if (min2 == -1 || nodes[i]->freq < nodes[min2]->freq)
            {
                min2 = i;
            }
        }

        if (min1 > min2)
        {
            int temp = min1;
            min1 = min2;
            min2 = temp;
        }

        Node *parent = createNode(0, nodes[min1]->freq + nodes[min2]->freq);
        parent->left = nodes[min1];
        parent->right = nodes[min2];

        nodes[min1] = parent;
        nodes[min2] = nodes[count - 1];
        count--;
    }

    return nodes[0];
}

/* 根据哈夫曼树生成编码表 */
void generateCodes(Node *root, char code[], int depth, char codes[BYTE_SIZE][MAX_CODE_LEN])
{
    if (root == NULL)
        return;

    if (isLeaf(root))
    {
        code[depth] = '\0';
        strcpy(codes[root->ch], code);
        return;
    }

    /* 左边记为 0 */
    code[depth] = '0';
    generateCodes(root->left, code, depth + 1, codes);

    /* 右边记为 1 */
    code[depth] = '1';
    generateCodes(root->right, code, depth + 1, codes);
}

/* 写入一个 bit 到文件 */
void writeBit(FILE *out, int bit, unsigned char *buffer, int *bitCount)
{
    *buffer = (*buffer << 1) | bit;
    (*bitCount)++;

    if (*bitCount == 8)
    {
        fwrite(buffer, 1, 1, out);
        *buffer = 0;
        *bitCount = 0;
    }
}

/* 把最后不足 8 位的 bit 补 0 写入 */
void flushBits(FILE *out, unsigned char *buffer, int *bitCount)
{
    if (*bitCount > 0)
    {
        *buffer <<= (8 - *bitCount);
        fwrite(buffer, 1, 1, out);
    }
}

/* 压缩文件 */
void compressFile(const char *inputFile, const char *outputFile)
{
    FILE *in = fopen(inputFile, "rb");
    if (in == NULL)
    {
        printf("无法打开输入文件：%s\n", inputFile);
        return;
    }

    FILE *out = fopen(outputFile, "wb");
    if (out == NULL)
    {
        printf("无法创建输出文件：%s\n", outputFile);
        fclose(in);
        return;
    }

    int freq[BYTE_SIZE] = {0};

    /* 第一次扫描：统计频率 */
    countFrequency(in, freq);

    /* 把频率表写入压缩文件头部，解压时靠它重建哈夫曼树 */
    fwrite(freq, sizeof(int), BYTE_SIZE, out);

    Node *root = buildHuffmanTree(freq);

    if (root == NULL)
    {
        fclose(in);
        fclose(out);
        return;
    }

    char codes[BYTE_SIZE][MAX_CODE_LEN] = {{0}};
    char code[MAX_CODE_LEN];

    /* 根据哈夫曼树生成每个字符的编码 */
    generateCodes(root, code, 0, codes);

    /* 第二次扫描：按原文件顺序，把字符替换成哈夫曼编码 */
    rewind(in);

    int ch;
    unsigned char buffer = 0;
    int bitCount = 0;

    while ((ch = fgetc(in)) != EOF)
    {
        char *huffCode = codes[(unsigned char)ch];

        for (int i = 0; huffCode[i] != '\0'; i++)
        {
            writeBit(out, huffCode[i] - '0', &buffer, &bitCount);
        }
    }

    flushBits(out, &buffer, &bitCount);

    freeTree(root);
    fclose(in);
    fclose(out);

    printf("压缩完成：%s -> %s\n", inputFile, outputFile);
}

/* 从文件中读取一个 bit */
int readBit(FILE *in, unsigned char *buffer, int *bitCount)
{
    if (*bitCount == 0)
    {
        int ch = fgetc(in);

        if (ch == EOF)
        {
            return -1;
        }

        *buffer = (unsigned char)ch;
        *bitCount = 8;
    }

    int bit = (*buffer & 0x80) ? 1 : 0;
    *buffer <<= 1;
    (*bitCount)--;

    return bit;
}

/* 解压文件 */
void decompressFile(const char *inputFile, const char *outputFile)
{
    FILE *in = fopen(inputFile, "rb");
    if (in == NULL)
    {
        printf("无法打开压缩文件：%s\n", inputFile);
        return;
    }

    FILE *out = fopen(outputFile, "wb");
    if (out == NULL)
    {
        printf("无法创建输出文件：%s\n", outputFile);
        fclose(in);
        return;
    }

    int freq[BYTE_SIZE] = {0};

    /* 读取压缩文件头部的频率表 */
    fread(freq, sizeof(int), BYTE_SIZE, in);

    Node *root = buildHuffmanTree(freq);

    if (root == NULL)
    {
        fclose(in);
        fclose(out);
        return;
    }

    /* 根据频率表计算原文件一共有多少个字符 */
    int totalChars = 0;
    for (int i = 0; i < BYTE_SIZE; i++)
    {
        totalChars += freq[i];
    }

    int decodedChars = 0;
    unsigned char buffer = 0;
    int bitCount = 0;

    Node *current = root;

    /* 逐 bit 读取压缩数据，在哈夫曼树上移动 */
    while (decodedChars < totalChars)
    {
        int bit = readBit(in, &buffer, &bitCount);

        if (bit == -1)
        {
            break;
        }

        if (bit == 0)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }

        /* 到达叶子节点，说明解出了一个字符 */
        if (isLeaf(current))
        {
            fputc(current->ch, out);
            decodedChars++;

            /* 回到根节点，继续解下一个字符 */
            current = root;
        }
    }

    freeTree(root);
    fclose(in);
    fclose(out);

    printf("解压完成：%s -> %s\n", inputFile, outputFile);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("用法：\n");
        printf("压缩：%s -c input.txt output.huf\n", argv[0]);
        printf("解压：%s -d input.huf output.txt\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0)
    {
        compressFile(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        decompressFile(argv[2], argv[3]);
    }
    else
    {
        printf("参数错误，请使用 -c 或 -d\n");
        return 1;
    }

    return 0;
}