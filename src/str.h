#ifndef STR_H_
#define STR_H_

#include <stdio.h>

#include <emmintrin.h>
#include <immintrin.h>
#include <x86intrin.h>
#include <stdint.h>

#define SIMD_OPT

const char space = ' ';

int ExtractWords(const char* input_file, const char* output_file);
void ArrayOutput(const char* output_file, size_t* numbers, size_t nNumbers);


static inline bool SimdStrcmp(const char* str1, const char* str2) {

    const uint8_t *adr1 = (const uint8_t *)str1;
    const uint8_t *adr2 = (const uint8_t *)str2;
    __m128i zeroes = _mm_setzero_si128();

    while (true) {
        __m128i word1 = _mm_loadu_si128((const __m128i *)adr1);
        __m128i word2 = _mm_loadu_si128((const __m128i *)adr2);

        uint16_t words_differ = (~(_mm_movemask_epi8(_mm_cmpeq_epi8(word1, word2)))) & 0xFFFF;

        uint16_t null1 = _mm_movemask_epi8(_mm_cmpeq_epi8(word1, zeroes));
        uint16_t null2 = _mm_movemask_epi8(_mm_cmpeq_epi8(word2, zeroes));

        uint16_t null_or_diff = null1 | null2 | words_differ;

        if (null_or_diff) {
            int first_zero = _tzcnt_u32(null_or_diff);
            uint16_t first_zero_mask = (uint16_t)(1 << first_zero);

            if (words_differ && first_zero_mask) return 1;

            if ((null1 & first_zero_mask) && (null2 & first_zero_mask)) {
                return 0;
            }

            return 1;
        }

        adr1 += 16;
        adr2 += 16;

    }
}

#endif //STR_H_