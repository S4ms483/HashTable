#include "table.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <immintrin.h>

#include "hash.h"
#include "str.h"
                
//FIXME - dndebug
//FIXME - strlen on intrinsics

HashTable* TableInit(size_t table_size, HashFunction function) {
    HashTable* table = (HashTable*)calloc(1, sizeof(HashTable));

    table->lists = (List**)calloc(table_size, sizeof(List*));
    for (size_t i = 0; i < table_size; i++) { table->lists[i] = ListInit(NULL, 0, 1); }
    
    table->nLists = table_size;
    table->function = function;

    return table;
}


HashTable* CreateTable(List* words, size_t table_size, HashFunction function) {
    HashTable* table = TableInit(table_size, function);

    uint32_t index = 0;
    size_t words_amount = words->lSize;
    char** words_array = words->array;

    table->tSize = words_amount;

    for (size_t i = 0; i < words_amount; i++) {
        index = table->function((const uint8_t*)words_array[i], strlen(words_array[i]));
        if (!ListAppend(table->lists[index], words_array[i])) (table->tSize)--;
    }

fprintf(stderr, "words amount = %lu\n", table->tSize);

    return table;
}


void MeasureLength(const char* length_file, size_t table_size, HashFunction function, List* words) {
    assert(length_file);
    
    HashTable* table = CreateTable(words, table_size, function);

    size_t* len_array = (size_t*)calloc(table_size, sizeof(size_t));
    List** lists = table->lists;
    for (size_t i = 0; i < table_size; i++) {
        len_array[i] = (lists[i])->lSize;
    }

    ArrayOutput(length_file, len_array, table_size);
    TableDestroy(table);
    free(len_array);
}


uint64_t MeasureTime(HashTable* table, List* search_list) {
    assert(table && search_list);

    size_t words_amount = search_list->lSize; 
    char** array = search_list->array;

    uint64_t start_time = __rdtsc();
    for (size_t i = 0; i < words_amount; i++) {
        TableSearch(table, array[i]);
    }
    uint64_t end_time = __rdtsc();

    TableDestroy(table);
    return end_time - start_time;
}


char* TableSearch(HashTable* table, char* word) {
    assert(table && word);

    uint32_t index = table->function((const uint8_t*)word, strlen(word));
    if (index >= (table->lists[index])->lSize) return NULL;
    return ListSearch(table->lists[index], word);
}


void TableDestroy(HashTable* table) {
    for (size_t i = 0; i < table->nLists; i++) {
        free((table->lists[i])->array);
        free(table->lists[i]);
    }

    free(table->lists);

    table->nLists = 0;
    table->function = NULL;

    free(table);
}