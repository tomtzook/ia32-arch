#pragma once

#include "types.h"


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
