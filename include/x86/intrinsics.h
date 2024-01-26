#pragma once

#include "types.h"


extern "C"
int memcmp(const void* s1, const void* s2, size_t size);

extern "C"
void memset(void* dest, uint8_t value, size_t size);

extern "C"
void* memcpy(void* dest, const void* src, size_t size);

extern "C"
size_t strlen(const char* s);

extern "C"
int strcmp(const char* s1, const char* s2);


static inline size_t bit_scan_forward(uint64_t value) noexcept {
    size_t size;
    asm volatile("bsf %1, %0"
            : "=r"(size) : "r" (value));
    return size;
}

static inline size_t bit_scan_reverse(uint64_t value) noexcept {
    size_t size;
    asm volatile("bsr %1, %0"
            : "=r"(size) : "r" (value));
    return size;
}
