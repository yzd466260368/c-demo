#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "word_stat.h"

static char *myStrdup(const char *str)
{
    char *newStr = (char *)malloc(strlen(str) + 1);

    if (newStr == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }

    strcpy(newStr, str);
    return newStr;
}

void initWordList(WordList *list)
{
    list->size = 0;
    list->capacity = 16;
    list->data = (WordFreq *)malloc(sizeof(WordFreq) * list->capacity);

    if (list->data == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }
}

void freeWordList(WordList *list)
{
    for (int i = 0; i < list->size; i++)
    {
        free(list->data[i].word);
    }

    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

void stringToLower(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

int findWord(const WordList *list, const char *word)
{
    for (int i = 0; i < list->size; i++)
    {
        if (strcmp(list->data[i].word, word) == 0)
        {
            return i;
        }
    }

    return -1;
}

void addWord(WordList *list, const char *word)
{
    int index = findWord(list, word);

    if (index != -1)
    {
        list->data[index].count++;
        return;
    }

    if (list->size >= list->capacity)
    {
        list->capacity *= 2;

        WordFreq *newData = (WordFreq *)realloc(
            list->data,
            sizeof(WordFreq) * list->capacity);

        if (newData == NULL)
        {
            printf("内存扩容失败\n");
            exit(1);
        }

        list->data = newData;
    }

    list->data[list->size].word = myStrdup(word);
    list->data[list->size].count = 1;
    list->size++;
}

int getWordCount(const WordList *list, const char *word)
{
    int index = findWord(list, word);

    if (index == -1)
    {
        return 0;
    }

    return list->data[index].count;
}

static int compareWordFreq(const void *a, const void *b)
{
    const WordFreq *w1 = (const WordFreq *)a;
    const WordFreq *w2 = (const WordFreq *)b;

    if (w1->count > w2->count)
    {
        return -1;
    }
    else if (w1->count < w2->count)
    {
        return 1;
    }

    return strcmp(w1->word, w2->word);
}

void sortWordList(WordList *list)
{
    qsort(list->data, list->size, sizeof(WordFreq), compareWordFreq);
}

void printTopN(const WordList *list, int n)
{
    if (n > list->size)
    {
        n = list->size;
    }

    for (int i = 0; i < n; i++)
    {
        printf("%-20s %d\n", list->data[i].word, list->data[i].count);
    }
}