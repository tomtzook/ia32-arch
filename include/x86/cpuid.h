#pragma once

#include "types.h"

// https://c9x.me/x86/html/file_module_x86_id_45.html
// https://wiki.osdev.org/CPUID
namespace x86 {

using cpuid_t = uint32_t;

#pragma pack(push, 1)

struct cpuid_regs_t {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
};

struct cpuid_eax01_t {
    union {
        struct {
            uint32_t stepping_id : 4;
            uint32_t model : 4;
            uint32_t family_id : 4;
            uint32_t processor_type : 2;
            uint32_t reserved0 : 2;
            uint32_t extended_model_id : 4;
            uint32_t extended_family_id : 8;
            uint32_t reserved1 : 4;
        } bits;
        uint32_t raw;
    } eax;
    union {
        uint32_t raw;
    } ebx;
    union {
        struct {
            uint32_t sse3 : 1;
            uint32_t reserved0 : 2;
            uint32_t monitor : 1;
            uint32_t ds_cpl : 1;
            uint32_t vmx : 1;
            uint32_t reserved1 : 1;
            uint32_t est : 1;
            uint32_t tm2 : 1;
            uint32_t reserved2 : 1;
            uint32_t cnxt_id : 1;
            uint32_t reserved3 : 21;
        } bits;
        uint32_t raw;
    } ecx;
    union {
        struct {
            uint32_t fpu : 1;
            uint32_t vme : 1;
            uint32_t de : 1;
            uint32_t pse : 1;
            uint32_t tsc : 1;
            uint32_t pae : 1;
            uint32_t mce : 1;
            uint32_t cx8 : 1;
            uint32_t apic : 1;
            uint32_t reserved0 : 1;
            uint32_t sep : 1;
            uint32_t pge : 1;
            uint32_t mca : 1;
            uint32_t cmov : 1;
            uint32_t pat : 1;
            uint32_t pse36 : 1;
            uint32_t psn : 1;
            uint32_t clfsh : 1;
            uint32_t ds : 1;
            uint32_t acpi : 1;
            uint32_t mmx : 1;
            uint32_t fxsr : 1;
            uint32_t sse : 1;
            uint32_t see2 : 1;
            uint32_t ss : 1;
            uint32_t htt : 1;
            uint32_t tm : 1;
            uint32_t reserved1 : 1;
            uint32_t pbe : 1;
        } bits;
        uint32_t raw;
    } edx;
};

#pragma pack(pop)

cpuid_regs_t cpuid(cpuid_t leaf, cpuid_t sub_leaf = 0) noexcept;

template<typename _regs_struct>
_regs_struct cpuid(cpuid_t leaf, cpuid_t sub_leaf = 0) noexcept {
    static_assert(sizeof(_regs_struct) == sizeof(cpuid_regs_t), "mismatch in registers size");
    return static_cast<_regs_struct>(cpuid(leaf, sub_leaf));
}

}
