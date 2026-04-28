#include <stdio.h>
#include <stdlib.h>

// #include "list.h"
#include "hash.h"
#include "table.h"

int main() {
    List* words_list = CreateList("data/input.txt", "data/output.txt");

    // MeasureLength("data/zeroHash.txt", 1, ZeroHash, words_list);
    // MeasureLength("data/firstLetter.txt", 'Z' - 'A' + 1, FirstLetter, words_list);
    // MeasureLength("data/wordLen.txt", 25, WordLen, words_list);
    // MeasureLength("data/sum503.txt", 503, LetterSumMod500, words_list);
    MeasureLength("data/sum4001.txt", 4001, LetterSumMod4001, words_list);
    // MeasureLength("data/rolHash.txt, 255, RolHash, words_list);
    // MeasureLength("data/crc32.txt", 4001, CRC32, words_list);

    ListDestroy(words_list);
}