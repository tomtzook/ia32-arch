#pragma once

#include "x86/common.h"


namespace x86::atomic {

static inline void mb() {
    asm volatile("mfence;" : : : "memory");
}

static inline void wmb() {
    asm volatile("sfence;" : : : "memory");
}

static inline void rmb() {
    asm volatile("lfence;" : : : "memory");
}

static inline uint8_t fetchadd8(volatile uint8_t* ptr, uint8_t value) {
    asm volatile("lock xaddb %0, %1"
            : "+r"(value), "+m"(*ptr)
            :
            : "cc"
            );
    return value;
}

static inline uint16_t fetchadd16(volatile uint16_t* ptr, uint16_t value) {
    asm volatile("lock xaddw %0, %1"
            : "+r"(value), "+m"(*ptr)
            :
            : "cc"
            );
    return value;
}

static inline uint32_t fetchadd32(volatile uint32_t* ptr, uint32_t value) {
    asm volatile("lock xaddl %0, %1"
            : "+r"(value), "+m"(*ptr)
            :
            : "cc"
            );
    return value;
}

static inline uint64_t fetchadd64(volatile uint64_t* ptr, uint64_t value) {
    asm volatile("lock xaddq %0, %1"
            : "+r"(value), "+m"(*ptr)
            :
            : "cc"
            );
    return value;
}

static inline uint8_t swap8(volatile uint8_t* ptr, uint8_t value) {
    asm volatile("lock xchgb %0, %1"
            : "+r"(value), "+m"(*ptr)
            :
            );
    return value;
}

static inline uint16_t swap16(volatile uint16_t* ptr, uint16_t value) {
    asm volatile("lock xchgw %0, %1"
            : "+r"(value), "+m"(*ptr)
            :
            );
    return value;
}

static inline uint32_t swap32(volatile uint32_t* ptr, uint32_t value) {
    asm volatile("lock xchgl %0, %1"
            : "+r"(value), "+m"(*ptr)
            :
            );
    return value;
}

static inline uint64_t swap64(volatile uint64_t* ptr, uint64_t value) {
    asm volatile("lock xchgq %0, %1"
            : "+r"(value), "+m"(*ptr)
            :
            );
    return value;
}

static inline bool cmpswap8(volatile uint8_t* ptr, uint8_t expect, uint8_t value) {
    bool result;
    asm volatile("lock cmpxchgb %3, %1"
            : "=@cce"(result), "+m"(*ptr), "+a"(expect)
            : "r"(value)
            : "memory", "cc"
            );
    return result;
}

static inline bool cmpswap16(volatile uint16_t* ptr, uint16_t expect, uint16_t value) {
    bool result;
    asm volatile("lock cmpxchgw %3, %1"
            : "=@cce"(result), "+m"(*ptr), "+a"(expect)
            : "r"(value)
            : "memory", "cc"
            );
    return result;
}

static inline bool cmpswap32(volatile uint32_t* ptr, uint32_t expect, uint32_t value) {
    bool result;
    asm volatile("lock cmpxchgl %3, %1"
            : "=@cce"(result), "+m"(*ptr), "+a"(expect)
            : "r"(value)
            : "memory", "cc"
            );
    return result;
}

static inline bool cmpswap64(volatile uint64_t* ptr, uint64_t expect, uint64_t value) {
    bool result;
    asm volatile("lock cmpxchgq %3, %1"
            : "=@cce"(result), "+m"(*ptr), "+a"(expect)
            : "r"(value)
            : "memory", "cc"
            );
    return result;
}

}