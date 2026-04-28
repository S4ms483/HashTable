#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>



int ExtractWords(const char* input_file, const char* output_file) {
    assert(input_file && output_file);

    FILE* input_ptr = fopen(input_file, "r");
    assert(input_ptr);

    FILE* output_ptr = fopen(output_file, "w");
    assert(output_file);

    int amount = 0;

    int symbol;
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


void ArrayOutput(const char* output_file, size_t* numbers, size_t nNumbers) {
    assert(output_file && numbers);

    FILE* output_ptr = fopen(output_file, "w");
    assert(output_file);

    for (size_t i = 0; i < nNumbers; i++) fprintf(output_ptr, "%lu %lu\n", i, numbers[i]);
}