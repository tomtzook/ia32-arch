#pragma once

#include "common.h"


#define DEFINE_MSR(_id, _name, _bits) \
constexpr x86::msr::id_t  _name## _id_t = _id; \
template<> struct x86::msr::msr_def_t<_name## _id_t> : public x86::msr::_msr_base_t<_name## _id_t> { \
    union {                              \
        struct {                         \
            _bits                                 \
        } bits;                          \
        x86::msr::value_t raw;\
    }; \
}; \
using _name## _t = x86::msr::msr_def_t<_name## _id_t>; \
static_assert(sizeof(_name## _t) == x86::msr::msr_def_size, "sizeof(_name## _t)");


namespace x86 {

namespace msr {

using id_t = uint32_t;
using value_t = uint64_t;
static constexpr size_t msr_def_size = sizeof(value_t);

#pragma pack(push, 1)

template<id_t _id>
struct _msr_base_t {
    static constexpr id_t id = _id;
};

template<id_t _id>
struct msr_def_t : public _msr_base_t<_id> {
    value_t raw;
};

template<typename _t>
struct is_msr_def : public meta::false_type {};
template<id_t _id>
struct is_msr_def<msr_def_t<_id>> : public meta::true_type {};

DEFINE_MSR(0xc0000100, ia32_fs_base,

)

DEFINE_MSR(0xc0000080, ia32_efer,
value_t sce : 1;
value_t reserved0 : 7;
value_t lme : 1;
value_t reserved1 : 1;
value_t lma : 1;
value_t nxe : 1;
value_t svme : 1;
value_t lmsle : 1;
value_t ffxsr : 1;
value_t tce : 1;
value_t reserved2 : 48;
)

DEFINE_MSR(0x3a, ia32_feature_ctrl,
uint64_t lock_bit : 1;
uint64_t vmx_smx : 1;
uint64_t vmx_no_smx : 1;
uint64_t reserved10 : 61;
)

DEFINE_MSR(0x1b, ia32_apic_base,
value_t reserved0: 8;
value_t bsp : 1;
value_t reserved1 : 1;
value_t extd : 1;
value_t global_enable : 1;
value_t base : 52;
)

DEFINE_MSR(0x480, ia32_vmx_basic,
value_t vmcs_revision : 31;
value_t must_be_zero : 1;
value_t vm_struct_size : 13;
value_t reserved0 : 3;
value_t physaddr_width_type : 1;
value_t dual_monitor_smi : 1;
value_t vmcs_mem_type : 3;
value_t ins_outs_vmexit_report : 1;
value_t vm_ctrls_fixed : 1;
value_t reserved1 : 8;
)

DEFINE_MSR(0x486, ia32_vmx_cr0_fixed0,
)

DEFINE_MSR(0x487, ia32_vmx_cr0_fixed1,
)

DEFINE_MSR(0x488, ia32_vmx_cr4_fixed0,
)

DEFINE_MSR(0x489, ia32_vmx_cr4_fixed1,
)

// [SDM 3 Appendix A.3.1]
DEFINE_MSR(0x481, ia32_vmx_pinbased_ctls,
value_t allowed0 : 32;
value_t allowed1 : 32;
);

DEFINE_MSR(0x48d, ia32_vmx_true_pinbased_ctls,
value_t allowed0 : 32;
value_t allowed1 : 32;
);

// [SDM 3 Appendix A.3.2]
DEFINE_MSR(0x482, ia32_vmx_procbased_ctls,
value_t allowed0 : 32;
value_t allowed1 : 32;
);

DEFINE_MSR(0x48e, ia32_vmx_true_procbased_ctls,
value_t allowed0 : 32;
value_t allowed1 : 32;
);

// [SDM 3 Appendix A.3.3]
DEFINE_MSR(0x48b, ia32_vmx_procbased_ctls2,
value_t allowed0 : 32;
value_t allowed1 : 32;
);

// [SDM 3 Appendix A.4]
DEFINE_MSR(0x483, ia32_vmx_exit_ctls,
value_t allowed0 : 32;
value_t allowed1 : 32;
);

DEFINE_MSR(0x48f, ia32_vmx_true_exit_ctls,
value_t allowed0 : 32;
value_t allowed1 : 32;
);

// [SDM 3 Appendix A.5]
DEFINE_MSR(0x484, ia32_vmx_entry_ctls,
value_t allowed0 : 32;
value_t allowed1 : 32;
);

DEFINE_MSR(0x490, ia32_vmx_true_entry_ctls,
value_t allowed0 : 32;
value_t allowed1 : 32;
);

#pragma pack(pop)

static inline value_t read(id_t id) noexcept {
    uint32_t low;
    uint32_t high;
    asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(id));
    return low | (static_cast<value_t>(high) << 32);
}

static inline void write(id_t id, value_t value) noexcept {
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    asm volatile ("wrmsr" : : "c"(id), "a"(low), "d"(high));
}

}

template<
        typename _t,
        typename meta::enable_if<
                msr::is_msr_def<_t>::value,
                bool>::type = 0
>
inline _t read() noexcept {
    static_assert(sizeof(_t) == msr::msr_def_size, "bad MSR size");
    _t t;
    t.raw = msr::read(_t::id);
    return t;
}

template<msr::id_t _id>
inline msr::msr_def_t<_id> read() noexcept {
    return read<msr::msr_def_t<_id>>();
}

template<
        typename _t,
        typename meta::enable_if<
                msr::is_msr_def<_t>::value,
                bool>::type = 0
>
inline void write(const _t& t) noexcept {
    static_assert(sizeof(_t) == msr::msr_def_size, "bad MSR size");
    msr::write(_t::id, t.raw);
}

template<msr::id_t _id>
inline void write(msr::msr_def_t<_id>& t) noexcept {
    return write<msr::msr_def_t<_id>>(t);
}

}
