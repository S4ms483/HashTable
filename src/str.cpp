#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stdint.h>

#include <ctype.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <x86intrin.h>
#include <stdint.h>


int ExtractWords(const char* input_file, const char* output_file) {
    assert(input_file && output_file);

    FILE* input_ptr = fopen(input_file, "r");
    assert(input_ptr);

    FILE* output_ptr = fopen(output_file, "w");
    assert(output_file);

    int amount = 0;

    int symbol = 0;
    bool is_alpha; 
    while (symbol != EOF) {
        symbol = fgetc(input_ptr);
        is_alpha = isalpha(symbol);

        if (!is_alpha) continue;

        while (is_alpha) {
            fputc(symbol, output_ptr);
            symbol = fgetc(input_ptr);
            is_alpha = isalpha(symbol);
        }

        amount++;
        fputc(space, output_ptr);
    }

    fclose(input_ptr);
    fclose(output_ptr);

    return amount;
}


int Strcmp(const char* str1, const char* str2) {
    assert(str1 && str2);

    for (size_t i = 0; i < 32; i++) {
        if (str1[i] != str2[i]) return 1; 
    }

    return 0;
}


void ArrayOutput(const char* output_file, size_t* numbers, size_t nNumbers) {
    assert(output_file && numbers);

    FILE* output_ptr = fopen(output_file, "w");
    assert(output_file);

    for (size_t i = 0; i < nNumbers; i++) fprintf(output_ptr, "%lu %lu\n", i, numbers[i]);
}