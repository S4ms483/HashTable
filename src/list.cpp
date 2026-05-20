#include "list.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>
#include <xmmintrin.h>

#include "str.h"


List* ListInit(char** array, size_t size, size_t capacity) {
    List* list = (List*)calloc(1, sizeof(List));

    if (!array) {list->array = (char**)calloc(1, sizeof(char*));}
    else {list->array = array;}

    assert(list->array);

    list->lSize = size;
    list->lCapacity = capacity;

    list->offsets = NULL;

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

    size_t l_size = list->lSize;
    char** array = list->array;

    #ifdef SIMD_CMP
    for (size_t i = 0; i < (l_size/4) * 4; i += 4) {
        int cmp1 = SimdStrcmp(array[i], word); 
        int cmp2 = SimdStrcmp(array[i + 1], word); 
        int cmp3 = SimdStrcmp(array[i + 2], word); 
        int cmp4 = SimdStrcmp(array[i + 3], word);

        if ((!cmp1) | (!cmp2) | (!cmp3) | (!cmp4)) return false;
    }

    for (size_t i = (l_size/4) * 4; i < l_size; i++) {
        bool cmp = SimdStrcmp(array[i], word);
        if (!cmp) return false;
    }

    #else 

    for (size_t i = 0; i < l_size; i++) {
        bool cmp = strcmp(array[i], word);
        if (!cmp) return false;
    }

    #endif

    if (list->lSize == list->lCapacity) {
        list->array = (char**)realloc(list->array, list->lCapacity * 2 * sizeof(char*));
        list->lCapacity = list->lCapacity * 2;
    }

    list->array[list->lSize] = (char*)word;
    (list->lSize)++;
    return true;
}


char* ListSearch(List* list, char* words_array, const char* word) {
    assert(list && word && words_array);

    size_t l_size = list->lSize;
    char** array = list->array;

    #ifdef SIMD_CMP

    for (size_t i = 0; i < (l_size/4) * 4; i += 4) {
        size_t* offsets = list->offsets;

        int cmp1 = SimdStrcmp(&words_array[offsets[i]], word); 
        int cmp2 = SimdStrcmp(&words_array[offsets[i + 1]], word); 
        int cmp3 = SimdStrcmp(&words_array[offsets[i + 2]], word); 
        int cmp4 = SimdStrcmp(&words_array[offsets[i + 3]], word);

        
        if ((!cmp1) | (!cmp2) | (!cmp3) | (!cmp4)) {
            size_t index = (!cmp2) + (!cmp3) * 2 + (!cmp4) * 3;
            return array[i + index];
        }
    }

    for (size_t i = (l_size/4) * 4; i < l_size; i++) {
        size_t offset = list->offsets[i];

        bool cmp = SimdStrcmp(&words_array[offset], word);
        if (!cmp) return array[i];
    }

    #else

    // for (size_t i = 0; i < l_size; i++) {
    //     size_t offset = list->offsets[i];

    //     bool cmp = strcmp(&words_array[offset], word);
    //     if (!cmp) return array[i];
    // }

    size_t prefetch_distance = 2;
    size_t* offsets = list->offsets;

    if (l_size >= prefetch_distance) {
        for (size_t i = 0; i < l_size - prefetch_distance; i++) {
            _mm_prefetch(&words_array[offsets[i + prefetch_distance]], _MM_HINT_T0);
            // uint8_t val = *(volatile uint8_t*)array[i + prefetch_distance];
            bool cmp = strcmp(&words_array[offsets[i]], word);
            if (!cmp) return array[i];
        }
        
        for (size_t i = l_size - prefetch_distance; i < l_size; i++) {
            bool cmp = strcmp(&words_array[offsets[i]], word);
            if (!cmp) return array[i];
        } 
    } else {
        for (size_t i = 0; i < l_size; i++) {
            bool cmp = strcmp(&words_array[offsets[i]], word);
            if (!cmp) return array[i];
        }
    }


    #endif

    return NULL;
}


void ListDestroy(List* list) {
    for (size_t i = 0; i < list->lSize; i++) free(list->array[i]);
    free(list->array);

    list->lSize = 0;
    list->lCapacity = 0;

    free(list->offsets);
    free(list);
}