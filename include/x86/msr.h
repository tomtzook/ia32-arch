#pragma once

#include "common.h"


#define DEFINE_MSR(_id, _name, _bits) \
constexpr id_t  _name## _id_t = _id; \
template<> struct msr_def_t<_name## _id_t> : public _msr_base_t<_name## _id_t> { \
    union {                              \
        struct {                         \
            _bits                                 \
        } bits;                          \
        value_t raw;\
    }; \
}; \
using _name## _t = msr_def_t<_name## _id_t>; \
static_assert(sizeof(_name## _t) == msr_def_size, "sizeof(_name## _t)");


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

DEFINE_MSR(0x1b, ia32_apic_base,
value_t reserved0: 8;
value_t bsp : 1;
value_t reserved1 : 2;
value_t global_enable : 1;
value_t base : 52;
)

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
