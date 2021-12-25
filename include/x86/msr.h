#pragma once

#include "common.h"


namespace x86 {

namespace msr {

using id_t = uint32_t;

#pragma pack(push, 1)

static constexpr size_t msr_def_size = sizeof(uintn_t);

template<id_t _id>
struct _msr_base_t {
    static constexpr id_t id = _id;
};

template<id_t _id>
struct msr_def_t : public _msr_base_t<_id> {
    uintn_t raw;
};

template<typename _t>
struct is_msr_def : public meta::false_type {};
template<id_t _id>
struct is_msr_def<msr_def_t<_id>> : public meta::true_type {};

template<>
struct msr_def_t<0xc0000080> : public _msr_base_t<0xc0000080> {
    union {
        struct {
            uintn_t sce : 1;
            uintn_t reserved0 : 7;
            uintn_t lme : 1;
            uintn_t reserved1 : 1;
            uintn_t lma : 1;
            uintn_t nxe : 1;
            uintn_t svme : 1;
            uintn_t lmsle : 1;
            uintn_t ffxsr : 1;
            uintn_t tce : 1;
#ifdef X86_64
            uintn_t reserved2 : 48;
#else
            uintn_t reserved2 : 16;
#endif
        } bits;
        uintn_t raw;
    };
};
using ia32_efer_t = msr_def_t<0xc0000080>;
static_assert(sizeof(ia32_efer_t) == msr_def_size, "sizeof(ia32_efer_t)");

#pragma pack(pop)

    static inline uintn_t read(id_t id) noexcept {
        uintn_t value;
        asm volatile("rdmsr" : "=A"(value) : "c"(id));
        return value;
    }

    static inline void write(id_t id, uintn_t value) noexcept {
#ifdef X86_64
        uint32_t low = value & 0xFFFFFFFF;
        uint32_t high = value >> 32;
        asm volatile ("wrmsr" : : "c"(id), "a"(low), "d"(high));
#else
        asm volatile ("wrmsr" : : "c"(id), "a"(value));
#endif
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
    return static_cast<_t>(msr::read(_t::id));
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
