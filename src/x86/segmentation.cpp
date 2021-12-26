
#include "x86/segmentation.h"


namespace x86::segmentation {

linear_address_t descriptor_t::base_address() const noexcept {
    return bits.base_address_low |
            (static_cast<linear_address_t>(bits.base_address_middle) << 16) |
            (static_cast<linear_address_t>(bits.base_address_high) << 24);
}

void descriptor_t::base_address(linear_address_t address) noexcept {
    bits.base_address_low = address & 0xffff;
    bits.base_address_middle = (address >> 16) & 0xff;
    bits.base_address_high = (address >> 24) & 0xff;
}

size_t descriptor_t::limit() const noexcept {
    return bits.limit_low | (static_cast<size_t>(bits.limit_high) << 16);
}

void descriptor_t::limit(size_t limit) noexcept {
    bits.limit_low = limit & 0xffff;
    bits.limit_high = (limit >> 16) & 0xf;
}

}
