#pragma once

#include "x86/common.h"
#include "x86/msr.h"

namespace x86 {
namespace mtrr {

// [SDM 3 11.11 "Table 11-8"]
enum class memory_type_t : uint8_t {
    uncacheable = 0,
    write_coombining = 1,
    write_through = 4,
    write_protected = 5,
    writeback = 6
};

static constexpr msr::id_t variable_base_msr_id = 0x200;
static constexpr msr::id_t variable_mask_msr_id = 0x201;

// [SDM 3 11.11.4.1]
static constexpr memory_type_t mtrr_disabled_memory_type = memory_type_t::uncacheable;
static constexpr memory_type_t memory_type_invalid = static_cast<memory_type_t>(-1);

#pragma pack(push, 1)

template<msr::id_t _msr_id, uint64_t _base, uint64_t _size>
struct fixed_t {
    static constexpr msr::id_t msr_id = _msr_id;
    static constexpr uint64_t base = _base;
    static constexpr uint64_t size = _size;

    struct value_t {
        union {
            uint8_t type[8];
            uint64_t raw;
        };
    };
    static_assert(sizeof(value_t) == 8, "sizeof(value_t)");

    // todo: loop read write
    // todo: iterator variable mtrr
};

template<typename _t>
struct is_fixed_mtrr_def : public meta::false_type {};
template<msr::id_t _msr_id, uint64_t _base, uint64_t _size>
struct is_fixed_mtrr_def<fixed_t<_msr_id, _base, _size>> : public meta::true_type {};

// [SDM 3 11.11.2.3]
struct variable_base_t {
    union {
        struct {
            uint64_t type : 8;
            uint64_t reserved0 : 4;
            uint64_t physbase : 52; // [12:63] depends on maxphysaddr
        } bits;
        uint64_t raw;
    };
};
static_assert(sizeof(variable_base_t) == 8, "sizeof(variable_base_t)");

struct variable_mask_t {
    union {
        struct {
            uint64_t reserved0 : 11;
            uint64_t v : 1;
            uint64_t physmask : 52; // [12:63] depends on maxphysaddr
        } bits;
        uint64_t raw;
    };
};
static_assert(sizeof(variable_mask_t) == 8, "sizeof(variable_mask_t)");

#pragma pack(pop)

// [SDM 3 11.11.2.2 "Table 11-9"]
using fixed_64k_00000_t = fixed_t<0x0250, 0x00000, 0x10000>;
using fixed_16k_80000_t = fixed_t<0x0258, 0x80000,  0x4000>;
using fixed_16k_a0000_t = fixed_t<0x0259, 0xa0000,  0x4000>;
using fixed_4k_c0000_t  = fixed_t<0x0268, 0xc0000,  0x1000>;
using fixed_4k_c8000_t  = fixed_t<0x0269, 0xc8000,  0x1000>;
using fixed_4k_d0000_t  = fixed_t<0x026a, 0xd0000,  0x1000>;
using fixed_4k_d8000_t  = fixed_t<0x026b, 0xd8000,  0x1000>;
using fixed_4k_e0000_t  = fixed_t<0x026c, 0xe0000,  0x1000>;
using fixed_4k_e8000_t  = fixed_t<0x026d, 0xe8000,  0x1000>;
using fixed_4k_f0000_t  = fixed_t<0x026e, 0xf0000,  0x1000>;
using fixed_4k_f8000_t  = fixed_t<0x026f, 0xf8000,  0x1000>;

struct mtrr_cache_t {
    static constexpr size_t max_fixed_mtrr = 16;
    static constexpr size_t max_variable_mtrr = 16;
    struct fixed_mtrr_t {
        memory_type_t type[8];
        physical_address_t base;
        uint64_t size;
    };
    struct variable_mtrr_t {
        bool enabled;
        memory_type_t type;
        uint64_t base;
        uint64_t mask;
        physical_address_t min;
        physical_address_t max;
    };

    bool enabled;
    memory_type_t default_type;

    bool fixed_mtrr_enabled;
    size_t fixed_mtrr_count;
    fixed_mtrr_t fixed_mtrrs[max_fixed_mtrr];

    size_t variable_mtrr_count;
    variable_mtrr_t variable_mtrrs[max_variable_mtrr];

    memory_type_t type_for_range(physical_address_t start, size_t size) const;
    memory_type_t type_for_2m(physical_address_t start) const;
    memory_type_t type_for_4k(physical_address_t start) const;

    static memory_type_t type_with_precedence(memory_type_t first, memory_type_t second);
};

static inline variable_base_t read_variable_base(size_t offset) {
    auto value = msr::read(variable_base_msr_id + offset * 2);
    return variable_base_t{.raw = value};
}

static inline variable_mask_t read_variable_mask(size_t offset) {
    auto value = msr::read(variable_mask_msr_id + offset * 2);
    return variable_mask_t{.raw = value};
}

mtrr_cache_t initialize_cache();

}

template<
        typename _t,
        typename meta::enable_if<
                mtrr::is_fixed_mtrr_def<_t>::value,
                bool>::type = 0
>
inline typename _t::value_t read() {
    using value_t = typename _t::value_t;
    value_t t;
    t.raw = msr::read(_t::msr_id);
    return t;
}

template<msr::id_t _msr_id, uint64_t _base, uint64_t _size>
inline mtrr::fixed_t<_msr_id, _base, _size> read() {
    return read<mtrr::fixed_t<_msr_id, _base, _size>>();
}

}
