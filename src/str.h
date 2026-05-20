#ifndef STR_H_
#define STR_H_

#include <stdio.h>

#include <emmintrin.h>
#include <immintrin.h>
#include <x86intrin.h>
#include <stdint.h>

// #define SIMD_CMP
// #define SIMD_LEN

const char space = ' ';

int ExtractWords(const char* input_file, const char* output_file);
void ArrayOutput(const char* output_file, size_t* numbers, size_t nNumbers);
int Strcmp(const char* str1, const char* str2);


static inline bool SimdStrcmp(const char* str1, const char* str2) {

    const uint8_t *adr1 = (const uint8_t *)str1;
    const uint8_t *adr2 = (const uint8_t *)str2;

    __m256i word1 = _mm256_loadu_si256((const __m256i *)adr1);
    __m256i word2 = _mm256_loadu_si256((const __m256i *)adr2);

    int words_differ = (~(_mm256_movemask_epi8(_mm256_cmpeq_epi16(word1, word2))));

    if (words_differ) return 1;

    return 0;
}


static inline size_t SimdStrlen(const char* str) {
    const uint8_t *adr = (const uint8_t *)str;

    __m256i zeroes = _mm256_setzero_si256();
    __m256i word = _mm256_loadu_si256((const __m256i *)adr);
    int null = _mm256_movemask_epi8(_mm256_cmpeq_epi16(word, zeroes));

    return (size_t)_tzcnt_u32((uint32_t)null);
}

#endif //STR_H_