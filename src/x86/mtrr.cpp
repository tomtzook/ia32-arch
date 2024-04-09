
#include "x86/msr.h"
#include "x86/paging/paging.h"
#include "x86/mtrr.h"

namespace x86 {
namespace mtrr {

template<typename _fixed_type>
static void load_fixed_mtrr(mtrr_cache_t& cache, size_t index) {
    auto msr_value = x86::read<_fixed_type>();

    auto& mtrr = cache.fixed_mtrrs[index];
    memcpy(mtrr.type, msr_value.type, sizeof(mtrr.type));
    mtrr.base = _fixed_type::base;
    mtrr.size = _fixed_type::size;
}

static void load_fixed_mtrrs(mtrr_cache_t& cache) noexcept {
    load_fixed_mtrr<fixed_64k_00000_t>(cache, 0);
    load_fixed_mtrr<fixed_16k_80000_t>(cache, 1);
    load_fixed_mtrr<fixed_16k_a0000_t>(cache, 2);
    load_fixed_mtrr<fixed_4k_c0000_t>(cache, 3);
    load_fixed_mtrr<fixed_4k_c8000_t>(cache, 4);
    load_fixed_mtrr<fixed_4k_d0000_t>(cache, 5);
    load_fixed_mtrr<fixed_4k_d8000_t>(cache, 6);
    load_fixed_mtrr<fixed_4k_e0000_t>(cache, 7);
    load_fixed_mtrr<fixed_4k_e8000_t>(cache, 8);
    load_fixed_mtrr<fixed_4k_f0000_t>(cache, 9);
    load_fixed_mtrr<fixed_4k_f8000_t>(cache, 10);

    cache.fixed_mtrr_count = 11;
}

static void load_variable_mtrrs(x86::msr::ia32_mtrr_cap_t& mtrr_cap, mtrr_cache_t& cache) {
    cache.variable_mtrr_count = mtrr_cap.bits.variable_range_count;
    for (int i = 0; i < cache.variable_mtrr_count; i++) {
        auto base = read_variable_base(i);
        auto mask = read_variable_mask(i);

        auto& mtrr = cache.variable_mtrrs[i];
        mtrr.enabled = mask.bits.v;
        mtrr.type = static_cast<memory_type_t>(base.bits.type);
        mtrr.base = base.bits.physbase;
        mtrr.mask = mask.bits.physmask;
        mtrr.min = mtrr.base * x86::paging::page_bits_4k;

        auto bit = bit_scan_forward(mtrr.mask * x86::paging::page_bits_4k);
        mtrr.max = mtrr.min + (1ull << bit) - 1;
    }
}

memory_type_t mtrr_cache_t::type_for_range(physical_address_t start, size_t size) const noexcept {
    // [SDM 3 11.11.4.1]
    // [SDM 3 11.11.7.1 "Example 11-4"]
    if (!enabled) {
        return mtrr_disabled_memory_type;
    }

    // align address to 4k
    start = start & ~(1 << x86::paging::page_bits_4k);
    auto end = (start + size) & ~(1 << x86::paging::page_bits_4k);

    auto last_type = type_for_4k(start);
    auto type = last_type;
    for (auto address = start + x86::paging::page_size_4k;
        address < end;
        address += x86::paging::page_size_4k
    ) {
        type = type_for_4k(address);
        type = type_with_precedence(type, last_type);
        if (memory_type_invalid == type) {
            // undefined
            return type;
        }

        last_type = type;
    }

    return type;
}

memory_type_t mtrr_cache_t::type_for_2m(physical_address_t start) const noexcept {
    if (!enabled) {
        return mtrr_disabled_memory_type;
    }

    // align address to 2m
    start = start & ~(1 << x86::paging::page_bits_2m);
    auto end = start + x86::paging::page_size_2m - 1;

    // fixed mtrr is only up to 1m, so only if start is 0 then fixed
    // are relevant.
    if (fixed_mtrr_enabled && start == 0x0) {
        auto last_type = memory_type_invalid;
        for (int i = 0; i < fixed_mtrr_count; ++i) {
            auto& mtrr = fixed_mtrrs[i];

            auto type = mtrr.type[i];
            if (last_type == memory_type_invalid) {
                last_type = type;
            }

            for (int j = 1; j < 8; ++j) {
                type = type_with_precedence(type, last_type);
                if (type == memory_type_invalid) {
                    return memory_type_invalid;
                }
            }
        }

        return last_type;
    }

    auto type = memory_type_invalid;
    for (int i = 0; i < variable_mtrr_count; ++i) {
        auto& mtrr = variable_mtrrs[i];
        if (!mtrr.enabled) {
            continue;
        }

        if (start >= mtrr.min && end <= mtrr.max) {
            if (type == memory_type_invalid) {
                type = mtrr.type;
            } else {
                type = type_with_precedence(type, mtrr.type);
                if (type == memory_type_invalid) {
                    return memory_type_invalid;
                }
            }
        }
    }

    if (type == memory_type_invalid) {
        type = default_type;
    }

    return type;
}

memory_type_t mtrr_cache_t::type_for_4k(physical_address_t start) const noexcept {
    // [SDM 3 11.11.7.1 "Example 11-5"]
    if (!enabled) {
        return mtrr_disabled_memory_type;
    }

    // align address to 4k
    start = start & ~(1 << x86::paging::page_bits_4k);
    auto end = start + x86::paging::page_size_4k - 1;

    if (fixed_mtrr_enabled && start < x86::paging::page_size_1m) {
        for (int i = 0; i < fixed_mtrr_count; ++i) {
            auto& mtrr = fixed_mtrrs[i];


            for (int j = 0; j < 8; ++j) {
                auto mtrr_start = mtrr.base + (mtrr.size * j);
                auto mtrr_end = mtrr_start + mtrr.size;

                if (start >= mtrr_start && end <= mtrr_end) {
                    return mtrr.type[j];
                }
            }
        }
    }

    for (int i = 0; i < variable_mtrr_count; ++i) {
        auto& mtrr = variable_mtrrs[i];
        if (!mtrr.enabled) {
            continue;
        }

        if ((start & mtrr.mask) == (mtrr.base & mtrr.mask)) {
            return mtrr.type;
        }
    }

    return default_type;
}

memory_type_t mtrr_cache_t::type_with_precedence(memory_type_t first, memory_type_t second) noexcept {
    // [SDM 3 11.11.4.1]
    if (first == second) {
        return first;
    }

    if (first == memory_type_t::uncacheable || second == memory_type_t::uncacheable) {
        return memory_type_t::uncacheable;
    }

    if (first == memory_type_t::writeback || second == memory_type_t::writeback) {
        if (first == memory_type_t::write_through || second == memory_type_t::write_through) {
            return memory_type_t::write_through;
        }
    }

    // undefined
    return memory_type_invalid;
}

mtrr_cache_t initialize_cache() noexcept {
    mtrr_cache_t cache{};

    auto mtrr_cap = x86::read<x86::msr::ia32_mtrr_cap_t>();
    auto mtrr_def = x86::read<x86::msr::ia32_mtrr_def_type_t>();

    if (mtrr_def.bits.enable) {
        cache.enabled = true;
        cache.default_type = static_cast<memory_type_t>(mtrr_def.bits.default_type);

        if (mtrr_cap.bits.fixed_range_supported && mtrr_def.bits.fixed_enable) {
            cache.fixed_mtrr_enabled = true;
            load_fixed_mtrrs(cache);
        } else {
            cache.fixed_mtrr_enabled = false;
        }

        load_variable_mtrrs(mtrr_cap, cache);
    } else {
        cache.enabled = false;
    }

    return cache;
}

}
}
