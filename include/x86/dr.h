#pragma once

#include "common.h"

// https://wiki.osdev.org/CPU_Registers_x86
namespace x86 {

#pragma pack(push, 1)

struct dr0_t {
    union {
        uintn_t raw;
    };

    dr0_t() : raw(0) {}
    explicit dr0_t(const uintn_t raw) : raw(raw) {}
};
static_assert(sizeof(dr0_t) == sizeof(uintn_t), "sizeof(dr0_t)");

struct dr1_t {
    union {
        uintn_t raw;
    };

    dr1_t() : raw(0) {}
    explicit dr1_t(const uintn_t raw) : raw(raw) {}
};
static_assert(sizeof(dr1_t) == sizeof(uintn_t), "sizeof(dr1_t)");

struct dr2_t {
    union {
        uintn_t raw;
    };

    dr2_t() : raw(0) {}
    explicit dr2_t(const uintn_t raw) : raw(raw) {}
};
static_assert(sizeof(dr2_t) == sizeof(uintn_t), "sizeof(dr2_t)");


struct dr3_t {
    union {
        uintn_t raw;
    };

    dr3_t() : raw(0) {}
    explicit dr3_t(const uintn_t raw) : raw(raw) {}
};
static_assert(sizeof(dr3_t) == sizeof(uintn_t), "sizeof(dr3_t)");

struct dr6_t {
    union {
        struct {
            uintn_t bp_0_cond : 1;
            uintn_t bp_1_cond : 1;
            uintn_t bp_2_cond : 1;
            uintn_t bp_3_cond : 1;
            uintn_t reserved0 : 6;
            uintn_t bld : 1;
            uintn_t smms : 1;
            uintn_t bd : 1;
            uintn_t bs : 1;
            uintn_t bt : 1;
            uintn_t rtm : 1;
        } bits;
        uintn_t raw;
    };

    dr6_t() : raw(0) {}
    explicit dr6_t(const uintn_t raw) : raw(raw) {}
};
static_assert(sizeof(dr6_t) == sizeof(uintn_t), "sizeof(dr6_t)");

enum class bp_condition_t : uintn_t {
    exec = 0b00,
    write = 0b01,
    io_rw = 0b10,
    readwrite = 0b11
};

enum class bp_length_t : uintn_t {
    byte = 0b00,
    word = 0b01,
    qword = 0b10,
    dword = 0b11
};

struct dr7_t {
    union {
        struct {
            uintn_t local_enable_bp_0 : 1;
            uintn_t global_enable_bp_0 : 1;
            uintn_t local_enable_bp_1 : 1;
            uintn_t global_enable_bp_1 : 1;
            uintn_t local_enable_bp_2 : 1;
            uintn_t global_enable_bp_2 : 1;
            uintn_t local_enable_bp_3 : 1;
            uintn_t global_enable_bp_3 : 1;
            uintn_t local_exact_bp_enable : 1;
            uintn_t global_exact_bp_enable : 1;
            uintn_t reserved0 : 1;
            uintn_t adv_debug_rtm : 1;
            uintn_t smie : 1;
            uintn_t general_detect_enable : 1;
            uintn_t reserved1 : 2;
            bp_condition_t condition_bp_0 : 2;
            bp_length_t length_bp_0 : 2;
            bp_condition_t condition_bp_1 : 2;
            bp_length_t length_bp_1 : 2;
            bp_condition_t condition_bp_2 : 2;
            bp_length_t length_bp_2 : 2;
            bp_condition_t condition_bp_3 : 2;
            bp_length_t length_bp_3 : 2;
            uintn_t reserved4 : 32;
        } bits;
        uintn_t raw;
    };

    dr7_t() : raw(0) {}
    explicit dr7_t(const uintn_t raw) : raw(raw) {}

    bool is_bp_active(unsigned idx) const;
    void set_bp(unsigned idx, bp_condition_t condition, bp_length_t len);
    void clear_bp(unsigned idx);
};
static_assert(sizeof(dr7_t) == sizeof(uintn_t), "sizeof(dr7_t)");

#pragma pack(pop)

allow_struct_read_write(dr0_t);

template<>
inline dr0_t read() {
    dr0_t reg;
    asm volatile("mov %%dr0, %0" : "=r"(reg.raw));
    return reg;
}

template<>
inline void write(const dr0_t& t) {
    asm volatile("mov %0, %%dr0" : : "r"(t.raw));
}

allow_struct_read_write(dr1_t);

template<>
inline dr1_t read() {
    dr1_t reg;
    asm volatile("mov %%dr1, %0" : "=r"(reg.raw));
    return reg;
}

template<>
inline void write(const dr1_t& t) {
    asm volatile("mov %0, %%dr1" : : "r"(t.raw));
}

allow_struct_read_write(dr2_t);

template<>
inline dr2_t read() {
    dr2_t reg;
    asm volatile("mov %%dr2, %0" : "=r"(reg.raw));
    return reg;
}

template<>
inline void write(const dr2_t& t) {
    asm volatile("mov %0, %%dr2" : : "r"(t.raw));
}

allow_struct_read_write(dr3_t);

template<>
inline dr3_t read() {
    dr3_t reg;
    asm volatile("mov %%dr3, %0" : "=r"(reg.raw));
    return reg;
}

template<>
inline void write(const dr3_t& t) {
    asm volatile("mov %0, %%dr3" : : "r"(t.raw));
}

allow_struct_read_write(dr6_t);

template<>
inline dr6_t read() {
    dr6_t reg;
    asm volatile("mov %%dr6, %0" : "=r"(reg.raw));
    return reg;
}

template<>
inline void write(const dr6_t& t) {
    asm volatile("mov %0, %%dr6" : : "r"(t.raw));
}

allow_struct_read_write(dr7_t);

template<>
inline dr7_t read() {
    dr7_t reg;
    asm volatile("mov %%dr7, %0" : "=r"(reg.raw));
    return reg;
}

template<>
inline void write(const dr7_t& t) {
    asm volatile("mov %0, %%dr7" : : "r"(t.raw));
}

}
