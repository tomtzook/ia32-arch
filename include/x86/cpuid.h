#pragma once

#include "common.h"


#define DEFINE_CPUID(_leaf, _subleaf, _name, _eax_bits, _ebx_bits, _ecx_bits, _edx_bits) \
template<> struct cpuid_def_t<_leaf, _subleaf> : public _cpuid_base_t<_leaf, _subleaf> { \
    union {                              \
        struct {                         \
            _eax_bits                    \
        } bits;                          \
        uint32_t raw;                    \
    } eax;                               \
    union {                              \
        struct {                         \
            _ebx_bits                    \
        } bits;                          \
        uint32_t raw;                    \
    } ebx;                               \
    union {                              \
        struct {                         \
            _ecx_bits                    \
        } bits;                          \
        uint32_t raw;                    \
    } ecx;                               \
    union {                              \
        struct {                         \
            _edx_bits                    \
        } bits;                          \
        uint32_t raw;                    \
    } edx;                               \
}; \
using _name## _t = cpuid_def_t<_leaf, _subleaf>; \
static_assert(sizeof(_name## _t) == cpuid_def_size, "sizeof(_name## _t)");


// https://c9x.me/x86/html/file_module_x86_id_45.html
// https://wiki.osdev.org/CPUID
namespace x86 {

using cpuid_t = uint32_t;

#pragma pack(push, 1)

static constexpr size_t cpuid_def_size = sizeof(int) * 4;

template<cpuid_t _leaf, cpuid_t _subleaf = 0>
struct _cpuid_base_t {
    static constexpr cpuid_t leaf = _leaf;
    static constexpr cpuid_t subleaf = _subleaf;
};

template<cpuid_t _leaf, cpuid_t _subleaf = 0>
struct cpuid_def_t : public _cpuid_base_t<_leaf, _subleaf> {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
};

template<typename T>
struct is_cpuid_def : public meta::false_type {};
template<cpuid_t _leaf, cpuid_t _subleaf>
struct is_cpuid_def<cpuid_def_t<_leaf, _subleaf>> : public meta::true_type {};

DEFINE_CPUID(0x1, 0x0, cpuid_eax01,
uint32_t stepping_id : 4;
uint32_t model : 4;
uint32_t family_id : 4;
uint32_t processor_type : 2;
uint32_t reserved0 : 2;
uint32_t extended_model_id : 4;
uint32_t extended_family_id : 8;
uint32_t reserved1 : 4;
,
uint32_t all : 32;
,
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
,
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
);

#pragma pack(pop)

cpuid_t max_supported_cpuid_leaf() noexcept;
bool is_cpuid_leaf_supported(cpuid_t leaf) noexcept;

template<
        typename _t,
        typename meta::enable_if<
                is_cpuid_def<_t>::value,
                bool>::type = 0
>
inline _t cpuid() noexcept {
    static_assert(sizeof(_t) == cpuid_def_size, "bad CPUID size");

    _t regs{};
    asm volatile("cpuid"
    : "=a"(regs.eax), "=b"(regs.ebx), "=c"(regs.ecx), "=d"(regs.edx)
    : "0"(_t::leaf), "2"(_t::subleaf));

    return regs;
}

template<cpuid_t _leaf, cpuid_t _subleaf = 0>
inline cpuid_def_t<_leaf, _subleaf> cpuid() noexcept {
    return cpuid<cpuid_def_t<_leaf, _subleaf>>();
}

}
