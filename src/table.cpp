#include "table.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <immintrin.h>

#include "hash.h"
#include "str.h"
                

HashTable* TableInit(size_t table_size, HashFunction function) {
    HashTable* table = (HashTable*)calloc(1, sizeof(HashTable));

    table->lists = (List**)calloc(table_size, sizeof(List*));
    for (size_t i = 0; i < table_size; i++) { table->lists[i] = ListInit(NULL, 0, 1); }
    
    table->nLists = table_size;
    table->function = function;
    table->words = NULL;

    return table;
}


HashTable* CreateTable(List* words, size_t table_size, HashFunction function) {
    HashTable* table = TableInit(table_size, function);

    uint32_t index = 0;
    size_t words_amount = words->lSize;
    char** words_array = words->array;

    table->nWords = words_amount;

    
    for (size_t i = 0; i < words_amount; i++) {
        #ifdef SIMD_LEN
        
        index = table->function((const uint8_t*)words_array[i], SimdStrlen(words_array[i]));
    
        #else 
    
        index = table->function((const uint8_t*)words_array[i], strlen(words_array[i]));
    
        #endif
        if (!ListAppend(table->lists[index % mod], words_array[i])) (table->nWords)--;
    }

    MoveLists(table);

fprintf(stderr, "words amount = %lu\n", table->nWords);

    return table;
}


void MoveLists(HashTable* table) {
    assert(table);

    char* words = (char*)calloc(table->nWords * wordLen, sizeof(char));
    size_t index = 0;
    size_t nLists = table->nLists;

    for (size_t i = 0; i < nLists; i++) {
        char** array = ((table->lists)[i])->array;
        size_t array_size = ((table->lists)[i])->lSize;

        size_t* indexes_array = (size_t*)calloc(array_size, sizeof(size_t));

        for (size_t j = 0; j < array_size; j++) {
            #ifdef SIMD_LEN
            size_t word_len = SimdStrlen(array[j]) + 1;
            #else
            size_t word_len = strlen(array[j]) + 1;
            #endif

            strcpy(&words[index], array[j]);
            indexes_array[j] = word_len;
            index += word_len;
        }

        ((table->lists)[i])->offsets = indexes_array;
    }

    table->words = words;
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
    uint64_t start_time;
    uint64_t end_time;

    uint64_t time_sum = 0;
    uint64_t n_searches = 100;

    for (uint64_t i = 0; i < n_searches; i++) {
        start_time = __rdtsc();
        for (size_t j = 0; j < words_amount; j++) {
            TableSearch(table, array[j]);
        }
        end_time = __rdtsc();

        time_sum += end_time - start_time;
    }

    TableDestroy(table);
    return time_sum/n_searches;
}


char* TableSearch(HashTable* table, char* word) {
    assert(table && word);

    #ifdef SIMD_LEN
    
    size_t len = SimdStrlen(word);

    #else 

    size_t len = strlen(word);

    #endif

    uint32_t hash = table->function((const uint8_t*)word, len);
    uint32_t index = hash % mod;

    return ListSearch(table->lists[index], table->words, word);
}


void TableDestroy(HashTable* table) {
    for (size_t i = 0; i < table->nLists; i++) {
        free((table->lists[i])->array);
        free(table->lists[i]->offsets);
        free(table->lists[i]);
    }

    free(table->lists);

    table->nLists = 0;
    table->function = NULL;

    free(table->words);
    free(table);
}