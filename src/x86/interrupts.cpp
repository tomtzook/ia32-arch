
#include "x86/interrupts.h"


namespace x86::interrupts {

uint32_t gate_descriptor_t::address() const noexcept {
    return bits.offset_low | (static_cast<uint32_t>(bits.offset_high) << 16);
}

void gate_descriptor_t::address(uint32_t address) noexcept {
    bits.offset_low = address & 0xffff;
    bits.offset_high = (address >> 16) & 0xffff;
}

}
