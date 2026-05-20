#ifndef TABLE_H_
#define TABLE_H_

#include <stdio.h>
#include <stdint.h>

#include "list.h"

typedef uint32_t (*HashFunction) (const uint8_t* data, size_t len);

typedef struct{
    List** lists;
    char* words;
    size_t nLists;
    size_t nWords;
    HashFunction function;
} HashTable;

const uint32_t mod = 4001;
const size_t wordLen = 32;

HashTable* TableInit(size_t table_size, HashFunction func);
HashTable* CreateTable(List* words, size_t table_size, HashFunction func);

void MoveLists(HashTable* table);
void TableSearch(HashTable* table, List* search_list, const char* output_file);

void MeasureLength(const char* length_file, size_t table_size, HashFunction func, List* words);
uint64_t MeasureTime(HashTable* table, List* search_list);

char* TableSearch(HashTable* table, char* word);
void TableDestroy(HashTable* table);

#endif //TABLE_H_