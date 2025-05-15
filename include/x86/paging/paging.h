#pragma once

#include "x86/common.h"

namespace x86::paging {

constexpr size_t page_bits_4k = 12;
constexpr size_t page_size_4k = 1ull << page_bits_4k;
constexpr size_t page_bits_1m = 20;
constexpr size_t page_size_1m = 1ull << page_bits_1m;
constexpr size_t page_bits_2m = 21;
constexpr size_t page_size_2m = 1ull << page_bits_2m;
constexpr size_t page_bits_4m = 22;
constexpr size_t page_size_4m = 1ull << page_bits_4m;
constexpr size_t page_bits_1g = 30;
constexpr size_t page_size_1g = 1ull << page_bits_1g;

constexpr size_t page_size = page_size_4k;

enum class mode_t {
    disabled,
    bit32,
    pae,
    ia32e
};

mode_t current_mode();

size_t max_physical_address_width();

constexpr bool is_page_aligned(const physical_address_t address) {
    return 0 == (address & (page_size - 1));
}

constexpr linear_address_t sign_extended(const linear_address_t address) {
    constexpr auto max_address_bits = 48; // todo: support 57, need to know how to check
    constexpr auto max_address_bit = (1ull << max_address_bits);

    const auto last_bit = address & max_address_bit;
    if (last_bit) {
        return address | ~(max_address_bit - 1);
    } else {
        return address & (max_address_bit - 1);
    }
}

constexpr bool is_canonical(const linear_address_t address) {
    return address == sign_extended(address);
}

static inline bool is_in_physical_address_width(const physical_address_t address) {
    const auto maxphysaddr = max_physical_address_width();
    const physical_address_t mask = ~((1ull << maxphysaddr) - 1);

    return (address & mask) == 0;
}

static inline physical_address_t align_in_max_physical_address_width(const physical_address_t address) {
    const auto maxphysaddr = max_physical_address_width();
    const physical_address_t mask = (1ull << maxphysaddr) - 1;

    return address & mask;
}

}
