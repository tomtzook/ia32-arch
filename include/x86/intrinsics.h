#pragma once

#include "types.h"


#ifdef __clang__

#define _arch_builtin_memcpy(_dest, _src, _size) __builtin_memcpy((_dest), (_src), (_size))
#define _arch_builtin_memset(_dest, _val, _size) __builtin_memset((_dest), (_val), (_size))

#else
#error "unsupported compiler"
#endif

inline size_t bit_scan_forward(uint64_t value) {
    size_t size;
    asm volatile("bsf %1, %0"
            : "=r"(size) : "r" (value));
    return size;
}

inline size_t bit_scan_reverse(uint64_t value) {
    size_t size;
    asm volatile("bsr %1, %0"
            : "=r"(size) : "r" (value));
    return size;
}
