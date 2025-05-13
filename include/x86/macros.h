#pragma once

#if defined(_WIN64) || defined(__x86_64__)
#define X86_64
#else
#error "only x86-64 is supported"
#endif

// #define packed __attribute__ ((packed));
// https://aaronbloomfield.github.io/pdr/book/x86-64bit-ccc-chapter.pdf
// i386 fastcall ASM calling convention.
// In GCC (system V), params are, from left to right :
// ecx, edx (for x86) or rdi, rsi, rdx, rcx, r8, r9 (x64)
// the rest are placed on the stack per normal conventions
#define fastcall __attribute__((fastcall))

#define array_size(array) sizeof(array) / sizeof(array[0])

#define bit(b) (1ULL << (b))
