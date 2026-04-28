#ifndef TABLE_H_
#define TABLE_H_

#include <stdio.h>
#include <stdint.h>

#include "list.h"

typedef uint32_t (*HashFunction) (const uint8_t* data, size_t len);

typedef struct{
    List** lists;
    size_t nLists;
    size_t tSize;
    HashFunction function;
} HashTable;

// typedefs for functions
HashTable* TableInit(size_t table_size, HashFunction func);
HashTable* CreateTable(List* words, size_t table_size, HashFunction func);

void TableSearch(HashTable* table, List* search_list, const char* output_file);
void MeasureLength(const char* length_file, size_t table_size, HashFunction func, List* words);
char* TableSearch(HashTable* table, char* word);
void TableDestroy(HashTable* table);

#endif //TABLE_H_