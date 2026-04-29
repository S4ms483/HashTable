#ifndef HASH_H_
#define HASH_H_

#include <stdio.h>
#include <stdint.h>

uint32_t ZeroHash(const uint8_t* data, size_t len);
uint32_t FirstLetter(const uint8_t* data, size_t len);
uint32_t WordLen(const uint8_t* data, size_t len);
uint32_t LetterSumMod503(const uint8_t* data, size_t len);
uint32_t LetterSumMod4001(const uint8_t* data, size_t len);
uint32_t RolHash(const uint8_t* data, size_t len);
uint32_t CRC32(const uint8_t* data, size_t len);
uint32_t Shift(uint32_t result);

#endif //HASH_H_