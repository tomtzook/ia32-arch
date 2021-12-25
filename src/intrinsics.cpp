#include "intrinsics.h"


void memset(void* dest, uint8_t value, size_t size) {
    auto* ptr = (uint8_t*) dest;
    while ((size--)) {
        *ptr = value;
        ++(ptr);
    }
}

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

size_t strlen(const char* s) {
    size_t count = 0;
    while (*s) {
        ++count;
        ++(s);
    }

    return count;
}
