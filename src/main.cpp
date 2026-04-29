#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// #include "list.h"
#include "hash.h"
#include "table.h"

int main() {
    List* words_list = CreateList("data/input.txt", "data/output.txt");

    // MeasureLength("data/zeroHash.txt", 1, ZeroHash, words_list);
    // MeasureLength("data/firstLetter.txt", 'Z' - 'A' + 1, FirstLetter, words_list);
    // MeasureLength("data/wordLen.txt", 503, WordLen, words_list);
    // MeasureLength("data/sum503.txt", 503, LetterSumMod503, words_list);
    // MeasureLength("data/sum4001.txt", 4001, LetterSumMod4001, words_list);
    // MeasureLength("data/rolHash.txt", 4001, RolHash, words_list);
    // MeasureLength("data/crc32.txt", 4001, CRC32, words_list);

    // HashTable* crc32_table = CreateTable(words_list, 4001, CRC32);
    // uint64_t time = MeasureTime(crc32_table, words_list);
    // printf("time = %lu\n", time);

    ListDestroy(words_list);
}