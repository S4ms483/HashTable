#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>

// FIXME hash(const uint8_t* data, size_t len)

int ZeroHash(const uint8_t* data, size_t len) {
    (void)data;
    (void)len;
    
    return 0;
}


int FirstLetter(const uint8_t* data, size_t len) {
    assert(data); 
    (void)len;
    char letter = (char)data[0];
    return letter >= 'a' ? letter - 'a' : letter - 'A';
}


int WordLen(const uint8_t* data, size_t len) {
    assert(data);
    return (int)len;
}


int LetterSumMod503(const uint8_t* data, size_t len) { 
    assert(data);
     
    // int word_len = strlen(word); 
    unsigned long long sum = 0;

    for (size_t i = 0; i < len; i++) {
        sum += data[i];
    }

    return (int)(sum % 503);
}


uint32_t LetterSumMod4001(const uint8_t* data, size_t len) { 
    assert(data);
     
    // size_t word_len = strlen(word); 
    uint32_t sum = 0;

    for (size_t i = 0; i < len; i++) {
        sum += data[i];
    }

    return (sum % 4001);
}


uint32_t RolHash(const uint8_t* data, size_t len) {
    assert(data);

    uint32_t letter = data[0];
    uint32_t result = letter;

    letter = data[1];

    for (size_t i = 2; i < len; i++) {
        result = Shift(result) ^ letter;
        letter = data[i];
    }

    return result;
}


uint32_t CRC32(const uint8_t* data, size_t len) {
    assert(data);
    uint32_t result = 5381;

    for (size_t i = 0; i < len; i++) {
        result = result ^ data[i];
        result = Shift(result);
    }

    return (result % 4001);
}


uint32_t Shift(uint32_t result) {
    size_t shift = 1; 
    return ((result << shift)) | (result >> (sizeof(result) * CHAR_BIT - shift)); 
} 