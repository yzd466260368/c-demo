#include <stdio.h>
#include <string.h>
#include "word_stat.h"
#include "file_reader.h"

int main(int argc, char const *argv[])
{

    /*
    统计文本中的单词数量
    统计每个单词出现频率
    找出最常出现的N个单词
    支持大小写转换
    支持输入单词，并在文件中统计输入单词的出现次数
    */

    char filename[256];
    int ignoreCase;
    int topN;
    int totalWords = 0;

    WordList list;
    initWordList(&list);

    if (argc >= 2)
    {
        strcpy(filename, argv[1]);
    }
    else
    {
        printf("请输入文件名：");
        scanf("%255s", filename);
    }

    printf("是否忽略大小写？1表示是，0表示否：");
    scanf("%d", &ignoreCase);

    if (!readFileAndCountWords(filename, &list, ignoreCase, &totalWords))
    {
        freeWordList(&list);
        return 1;
    }

    sortWordList(&list);

    printf("\n========== 统计结果 ==========\n");
    printf("单词总数：%d\n", totalWords);
    printf("不同单词数量：%d\n", list.size);

    printf("\n请输入要显示出现频率最高的前 N 个单词：");
    scanf("%d", &topN);

    printf("\n========== 高频单词 ==========\n");
    printTopN(&list, topN);

    char searchWord[256];

    while (1)
    {
        printf("\n请输入要查询的单词，输入 0 退出：");
        scanf("%255s", searchWord);

        if (strcmp(searchWord, "0") == 0)
        {
            break;
        }

        if (ignoreCase)
        {
            stringToLower(searchWord);
        }

        int count = getWordCount(&list, searchWord);

        if (count > 0)
        {
            printf("单词 \"%s\" 出现了 %d 次\n", searchWord, count);
        }
        else
        {
            printf("单词 \"%s\" 没有出现\n", searchWord);
        }
    }

    freeWordList(&list);

    return 0;
}
