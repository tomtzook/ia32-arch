#pragma once

#if defined(_WIN64) || defined(__x86_64__)
#define X86_64
#else
#define X86
#endif

#define packed __attribute__ ((packed));
// https://aaronbloomfield.github.io/pdr/book/x86-64bit-ccc-chapter.pdf
// i386 fastcall ASM calling convention.
// In GCC (system V), params are, from left to right :
// ecx, edx (for x86) or rdi, rsi, rdx, rcx, r8, r9 (x64)
// the rest are placed on the stack per normal conventions
#define fastcall __attribute__((fastcall))

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

#define BIT(b) (1ULL << (b))

#define REPEATARG1(arg) arg
#define REPEATARG2(arg) arg, REPEATARG1(arg)
#define REPEATARG3(arg) arg, REPEATARG2(arg)
#define REPEATARG4(arg) arg, REPEATARG3(arg)
#define REPEATARG5(arg) arg, REPEATARG4(arg)
#define REPEATARG6(arg) arg, REPEATARG5(arg)
#define REPEATARG7(arg) arg, REPEATARG6(arg)
#define REPEATARG8(arg) arg, REPEATARG7(arg)
#define REPEATARG9(arg) arg, REPEATARG8(arg)
#define REPEATARG10(arg) arg, REPEATARG9(arg)
