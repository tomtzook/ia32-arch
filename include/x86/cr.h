#pragma once

#include "common.h"

// https://wiki.osdev.org/CPU_Registers_x86
namespace x86 {

#pragma pack(push, 1)

struct cr0_t {
    union {
        struct {
            uintn_t protection_enable : 1;
            uintn_t monitor_coprocessor : 1;
            uintn_t emulate_fpu : 1;
            uintn_t task_switched : 1;
            uintn_t extension_type : 1;
            uintn_t numeric_error : 1;
            uintn_t reserved1 : 10;
            uintn_t write_protect : 1;
            uintn_t reserved2 : 1;
            uintn_t alignment_mask : 1;
            uintn_t reserved3 : 10;
            uintn_t not_write_through : 1;
            uintn_t cache_disable : 1;
            uintn_t paging_enable : 1;
#ifdef X86_64
            uintn_t reserved4 : 32;
#endif
        } bits;
        uintn_t raw;
    };

    cr0_t() noexcept : raw(0) {}
    cr0_t(uintn_t raw) noexcept : raw(raw) {}
};
static_assert(sizeof(cr0_t) == sizeof(uintn_t), "sizeof(cr0_t)");
allow_struct_read_write(cr0_t);

struct cr3_t {
    union {
        struct {
            uintn_t ignored0 : 3;
            uintn_t oage_write_through : 1;
            uintn_t page_cache_disable : 1;
            uintn_t ignored1 : 7;
#ifdef X86_64
            uintn_t page_table_address : 52;
#else
            uintn_t page_table_address : 20;
#endif
        } bits;
        uintn_t raw;
    };

    cr3_t() noexcept : raw(0) {}
    cr3_t(uintn_t raw) noexcept : raw(raw) {}
};
static_assert(sizeof(cr3_t) == sizeof(uintn_t), "sizeof(cr3_t)");
allow_struct_read_write(cr3_t);

struct cr4_t {
    union {
        struct {
            uintn_t virtual_mode_extensions : 1;
            uintn_t protected_mode_virtual_interrupts : 1;
            uintn_t timestamp_disable : 1;
            uintn_t debugging_extensions : 1;
            uintn_t page_size_extensions : 1;
            uintn_t physical_address_extension : 1;
            uintn_t machine_check_enable : 1;
            uintn_t page_global_enable : 1;
            uintn_t performance_monitoring_counter_enable : 1;
            uintn_t os_fxsave_fxrstor_support : 1;
            uintn_t os_xmm_exception_support : 1;
            uintn_t usermode_instruction_prevention : 1;
            uintn_t reserved1 : 1;
            uintn_t vmx_enable : 1;
            uintn_t smx_enable : 1;
            uintn_t reserved2 : 1;
            uintn_t fsgsbase_enable : 1;
            uintn_t pcid_enable : 1;
            uintn_t os_xsave : 1;
            uintn_t reserved3 : 1;
            uintn_t smep_enable : 1;
            uintn_t smap_enable : 1;
            uintn_t protection_key_enable : 1;
#ifdef X86_64
            uintn_t reserved4 : 32;
#endif
        } bits;
        uintn_t raw;
    };

    cr4_t() noexcept : raw(0) {}
    cr4_t(uintn_t raw) noexcept : raw(raw) {}
};
static_assert(sizeof(cr4_t) == sizeof(uintn_t), "sizeof(cr4_t)");
allow_struct_read_write(cr4_t);

#pragma pack(pop)

template<>
inline cr0_t read() noexcept {
    cr0_t reg;
    asm volatile("mov %%cr0, %0" : "=r"(reg.raw));
    return reg;
}

template<>
inline void write(const cr0_t& t) noexcept {
    asm volatile("mov %0, %%cr0" : : "r"(t.raw));
}

template<>
inline cr3_t read() noexcept {
    cr3_t reg;
    asm volatile("mov %%cr3, %0" : "=r"(reg.raw));
    return reg;
}

template<>
inline void write(const cr3_t& t) noexcept {
    asm volatile("mov %0, %%cr3" : : "r"(t.raw));
}

template<>
inline cr4_t read() noexcept {
    cr4_t reg;
    asm volatile("mov %%cr4, %0" : "=r"(reg.raw));
    return reg;
}

template<>
inline void write(const cr4_t& t) noexcept {
    asm volatile("mov %0, %%cr4" : : "r"(t.raw));
}

}
