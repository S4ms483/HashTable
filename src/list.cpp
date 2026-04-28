#include "list.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"

List* ListInit(char** array, size_t size, size_t capacity) {
    List* list = (List*)calloc(1, sizeof(List));

    if (!array) {list->array = (char**)calloc(1, sizeof(char*));}
    else {list->array = array;}

    assert(list->array);

    list->lSize = size;
    list->lCapacity = capacity;

    return list;
}


List* CreateList(const char* input_file, const char* output_file) {
    assert(input_file && output_file);

    size_t words_amount = (size_t)ExtractWords(input_file, output_file);

    char** words_array = (char**)calloc(words_amount, sizeof(char*));
    assert(words_array);

    FILE* output_ptr = fopen(output_file, "r");
    assert(output_file);
    
    char* word = NULL;
    size_t len = 0;
    ssize_t read;
    for (size_t i = 0; i < words_amount; i++) {
        read = getdelim(&word, &len, ' ', output_ptr);
        assert(read != -1);
        
        char* result = (char*)calloc(len, sizeof(char));
        result = strcpy(result, word);
        words_array[i] = result;
    }
    free(word);

    List* list = ListInit(words_array, words_amount, words_amount);

    return list;
}


bool ListAppend(List* list, char* word) {
    assert(word && list->array && list->lCapacity);

    for (size_t i = 0; i < list->lSize; i++) {
        if (!strcmp(list->array[i], word)) return false;
    }

    if (list->lSize == list->lCapacity) {
        list->array = (char**)realloc(list->array, list->lCapacity * 2 * sizeof(char*));
        list->lCapacity = list->lCapacity * 2;
    }

    list->array[list->lSize] = (char*)word;
    (list->lSize)++;
    return true;
}


char* ListSearch(List* list, const char* word) {
    assert(list && word);

    size_t list_size = list->lSize;
    for (size_t i = 0; i < list_size; i++) {
        if (!strcmp(list->array[i], word)) return list->array[i];
    }

    return NULL;
}


void ListDestroy(List* list) {
    for (size_t i = 0; i < list->lSize; i++) free(list->array[i]);
    free(list->array);

    list->lSize = 0;
    list->lCapacity = 0;

    free(list);
}