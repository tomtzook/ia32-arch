#pragma once

#include "common.h"

namespace x86 {

#pragma pack(push, 1)

struct rflags_t {
    union {
        struct {
            uintn_t carry_flag : 1;
            uintn_t reserved0 : 1;
            uintn_t parity_flag : 1;
            uintn_t reserved1 : 1;
            uintn_t zero_flag : 1;
            uintn_t sign_flag : 1;
            uintn_t trap_flag : 1;
            uintn_t interrupt_enable_flag : 1;
            uintn_t direction_flag : 1;
            uintn_t overflow_flag : 1;
            uintn_t io_privilege_level : 2;
            uintn_t nested_task : 1;
            uintn_t reserved2 : 1;
            uintn_t resume_flag : 1;
            uintn_t virtual_8086 : 1;
            uintn_t alignment_check : 1;
            uintn_t virtual_interrupt_flag : 1;
            uintn_t virtual_interrupt_pending : 1;
            uintn_t id_flag : 1;
            uintn_t reserved3: 42;
        } bits;
        uintn_t raw;
    };

    rflags_t() : raw(0) {}
    rflags_t(uintn_t raw) : raw(raw) {}
};
static_assert(sizeof(rflags_t) == sizeof(uintn_t), "sizeof(rflags_t)");

#pragma pack(pop)

allow_struct_read_write(rflags_t);

template<>
inline rflags_t read() {
    rflags_t reg;
    asm volatile(
        "pushfq     \n\t\
         pop %%rax  \n\t\
         mov %%rax, %0"
        : "=r"(reg.raw));
    return reg;
}

template<>
inline void write(const rflags_t& t) {
    asm volatile(
        "mov %0, %%rax \n\t\
         push %%rax    \n\t\
         popfq"
    : : "r"(t.raw));
}

}
