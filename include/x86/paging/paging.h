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

mode_t current_mode() noexcept;

size_t max_physical_address_width() noexcept;

constexpr bool is_page_aligned(physical_address_t address) {
    return 0 == (address & (page_size - 1));
}

}
