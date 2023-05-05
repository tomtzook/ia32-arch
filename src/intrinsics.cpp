#pragma once

#include "intrinsics.h"

// TODO: replace implementations to use some x86 instructions for dword/qword granularity

extern "C"
int memcmp(const void* s1, const void* s2, size_t size) {
    auto* ptr_s1 = (uint8_t*) s1;
    auto* ptr_s2 = (uint8_t*) s2;
    while (size-- > 0) {
        if (*ptr_s1++ != *ptr_s2++)
            return ptr_s1[-1] < ptr_s2[-1] ? -1 : 1;
    }
    return 0;
}

extern "C"
void memset(void* dest, uint8_t value, size_t size) {
    auto* ptr = (uint8_t*) dest;
    while ((size--)) {
        *ptr = value;
        ++(ptr);
    }
}

extern "C"
void* memcpy(void* dest, const void* src, size_t size) {
    auto* ptr_src = (uint8_t*) src;
    auto* ptr_dest = (uint8_t*) dest;
    while ((size--)) {
        *ptr_dest = *ptr_src;
        ++(ptr_src);
        ++(ptr_dest);
    }

    return dest;
}

extern "C"
size_t strlen(const char* s) {
    size_t count = 0;
    while (*s) {
        ++count;
        ++(s);
    }

    return count;
}

extern "C"
int strcmp(const char* s1, const char* s2) {
    while (*s1) {
        if (*s1 != *s2) {
            break;
        }
        ++(s1);
        ++(s2);
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

