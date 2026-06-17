#ifndef FILE_READER_H
#define FILE_READER_H

#include "word_stat.h"

int readFileAndCountWords(
    const char *filename,
    WordList *list,
    int ignoreCase,
    int *totalWords);

#endif