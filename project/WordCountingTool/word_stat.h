#ifndef WORD_STAT_H
#define WORD_STAT_H

typedef struct
{
    char *word; // 单词
    int count;  // 出现次数
} WordFreq;

typedef struct
{
    WordFreq *data; // 动态数组
    int size;       // 当前单词数量
    int capacity;   // 数组容量
} WordList;

// 初始化和释放
void initWordList(WordList *list);
void freeWordList(WordList *list);

// 添加、查找、查询
void addWord(WordList *list, const char *word);
int findWord(const WordList *list, const char *word);
int getWordCount(const WordList *list, const char *word);

// 排序和输出
void sortWordList(WordList *list);
void printTopN(const WordList *list, int n);

// 字符串转小写
void stringToLower(char *str);

#endif