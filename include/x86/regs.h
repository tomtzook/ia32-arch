#pragma once

#include "x86/common.h"

namespace x86 {

// todo: do we need to declare each register as clobbered?  (:: "al")

inline __attribute__((always_inline)) uint8_t read_al() {
    uint8_t reg;
    asm volatile("mov %%al, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint8_t read_ah() {
    uint8_t reg;
    asm volatile("mov %%ah, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint16_t read_ax() {
    uint16_t reg;
    asm volatile("mov %%ax, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint32_t read_eax() {
    uint32_t reg;
    asm volatile("mov %%eax, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rax() {
    uint64_t reg;
    asm volatile("mov %%rax, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint8_t read_bl() {
    uint8_t reg;
    asm volatile("mov %%bl, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint8_t read_bh() {
    uint8_t reg;
    asm volatile("mov %%bh, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint16_t read_bx() {
    uint16_t reg;
    asm volatile("mov %%bx, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint32_t read_ebx() {
    uint32_t reg;
    asm volatile("mov %%ebx, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rbx() {
    uint64_t reg;
    asm volatile("mov %%rbx, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint8_t read_cl() {
    uint8_t reg;
    asm volatile("mov %%cl, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint8_t read_ch() {
    uint8_t reg;
    asm volatile("mov %%ch, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint16_t read_cx() {
    uint16_t reg;
    asm volatile("mov %%cx, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint32_t read_ecx() {
    uint32_t reg;
    asm volatile("mov %%ecx, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rcx() {
    uint64_t reg;
    asm volatile("mov %%rcx, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint8_t read_dl() {
    uint8_t reg;
    asm volatile("mov %%dl, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint8_t read_dh() {
    uint8_t reg;
    asm volatile("mov %%dh, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint16_t read_dx() {
    uint16_t reg;
    asm volatile("mov %%dx, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint32_t read_edx() {
    uint32_t reg;
    asm volatile("mov %%edx, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rdx() {
    uint64_t reg;
    asm volatile("mov %%rdx, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint8_t read_bpl() {
    uint8_t reg;
    asm volatile("mov %%bpl, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint16_t read_bp() {
    uint16_t reg;
    asm volatile("mov %%bp, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint32_t read_ebp() {
    uint32_t reg;
    asm volatile("mov %%ebp, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rbp() {
    uint64_t reg;
    asm volatile("mov %%rbp, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint8_t read_spl() {
    uint8_t reg;
    asm volatile("mov %%spl, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint16_t read_sp() {
    uint16_t reg;
    asm volatile("mov %%sp, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint32_t read_esp() {
    uint32_t reg;
    asm volatile("mov %%esp, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rsp() {
    uint64_t reg;
    asm volatile("mov %%rsp, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint8_t read_sil() {
    uint8_t reg;
    asm volatile("mov %%sil, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint16_t read_si() {
    uint16_t reg;
    asm volatile("mov %%si, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint32_t read_esi() {
    uint32_t reg;
    asm volatile("mov %%esi, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rsi() {
    uint64_t reg;
    asm volatile("mov %%rsi, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint8_t read_dil() {
    uint8_t reg;
    asm volatile("mov %%dil, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint16_t read_di() {
    uint16_t reg;
    asm volatile("mov %%di, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint32_t read_edi() {
    uint32_t reg;
    asm volatile("mov %%edi, %0" : "=r"(reg));
    return reg;
}

inline __attribute__((always_inline)) uint64_t read_rdi() {
    uint64_t reg;
    asm volatile("mov %%rdi, %0" : "=r"(reg));
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

inline __attribute__((always_inline)) uint64_t read_rip() {
    uint64_t reg;
    asm volatile("lea (%%rip), %0" : "=r"(reg));
    return reg;
}

}
