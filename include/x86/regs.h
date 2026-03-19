#pragma once

#include "x86/common.h"

namespace x86 {

inline __attribute__((always_inline)) uint64_t read_rax() {
    uint64_t reg;
    asm volatile("mov %%rax, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rbx() {
    uint64_t reg;
    asm volatile("mov %%rbx, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rcx() {
    uint64_t reg;
    asm volatile("mov %%rcx, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rdx() {
    uint64_t reg;
    asm volatile("mov %%rdx, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_r8() {
    uint64_t reg;
    asm volatile("mov %%r8, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_r9() {
    uint64_t reg;
    asm volatile("mov %%r9, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_r10() {
    uint64_t reg;
    asm volatile("mov %%r10, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_r11() {
    uint64_t reg;
    asm volatile("mov %%r11, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_r12() {
    uint64_t reg;
    asm volatile("mov %%r12, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_r13() {
    uint64_t reg;
    asm volatile("mov %%r13, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_r14() {
    uint64_t reg;
    asm volatile("mov %%r14, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_r15() {
    uint64_t reg;
    asm volatile("mov %%r15, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rsi() {
    uint64_t reg;
    asm volatile("mov %%rsi, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rdi() {
    uint64_t reg;
    asm volatile("mov %%rdi, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rbp() {
    uint64_t reg;
    asm volatile("mov %%rbp, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rsp() {
    uint64_t reg;
    asm volatile("mov %%rsp, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rip() {
    uint64_t reg;
    asm volatile("lea (%%rip), %0" : "=r"(reg));
    return reg;
}

}
