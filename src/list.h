#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>

typedef struct{
    char** array;
    size_t lSize;
    size_t lCapacity;
} List;


List* ListInit(char** array, size_t size, size_t capacity);
List* CreateList(const char* input_file, const char* output_file);
char* ListSearch(List* list, const char* word);
bool ListAppend(List* list, char* word);
void ListDestroy(List* list);

#endif //LIST_H_