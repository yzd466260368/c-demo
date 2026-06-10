#include <stdio.h>
#include <ctype.h>
#include "file_reader.h"
#include "word_stat.h"

#define WORD_BUF_SIZE 256

static int isWordChar(int ch)
{
    return isalnum((unsigned char)ch) || ch == '_';
}

int readFileAndCountWords(
    const char *filename,
    WordList *list,
    int ignoreCase,
    int *totalWords)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("文件打开失败：%s\n", filename);
        return 0;
    }

    char word[WORD_BUF_SIZE];
    int len = 0;
    int ch;

    while ((ch = fgetc(fp)) != EOF)
    {
        if (isWordChar(ch))
        {
            if (len < WORD_BUF_SIZE - 1)
            {
                word[len++] = (char)ch;
            }
        }
        else
        {
            if (len > 0)
            {
                word[len] = '\0';

                if (ignoreCase)
                {
                    stringToLower(word);
                }

                addWord(list, word);
                (*totalWords)++;

                len = 0;
            }
        }
    }

    if (len > 0)
    {
        word[len] = '\0';

        if (ignoreCase)
        {
            stringToLower(word);
        }

        addWord(list, word);
        (*totalWords)++;
    }

    fclose(fp);
    return 1;
}