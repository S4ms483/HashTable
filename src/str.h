#ifndef STR_H_
#define STR_H_

#include <stdio.h>

const int bufferLen = 16;
const char space = ' ';

int ExtractWords(const char* input_file, const char* output_file);
void ArrayOutput(const char* output_file, size_t* numbers, size_t nNumbers);

#endif //STR_H_