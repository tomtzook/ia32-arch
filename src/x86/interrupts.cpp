
#include "x86/interrupts.h"


namespace x86::interrupts {

uint32_t descriptor_t::address() const {
    return bits.offset_low | (static_cast<uint32_t>(bits.offset_high) << 16);
}

void descriptor_t::address(uint32_t address) {
    bits.offset_low = address & 0xffff;
    bits.offset_high = (address >> 16) & 0xffff;
}

uint64_t descriptor64_t::address() const {
    return static_cast<uint64_t>(low.bits.offset_low) |
        (static_cast<uint64_t>(low.bits.offset_high) << 16) |
            (static_cast<uint64_t>(high.bits.offset_upper) << 32);
}

void descriptor64_t::address(uint64_t address) {
    low.bits.offset_low = address & 0xffff;
    low.bits.offset_high = (address >> 16) & 0xffff;
    high.bits.offset_upper = (address >> 32) & 0xffff;
}

table64_t::table64_t(idtr_t idtr)
    : m_idtr(idtr)
{}

const void* table64_t::base_address() const {
    return reinterpret_cast<const void *>(m_idtr.base_address);
}

void* table64_t::base_address() {
    return reinterpret_cast<void*>(m_idtr.base_address);
}

size_t table64_t::limit() const {
    return m_idtr.limit;
}

size_t table64_t::count() const {
    return (limit() + 1) / sizeof(descriptor64_t);
}

const descriptor64_t& table64_t::operator[](size_t index) const {
    return static_cast<const descriptor64_t*>(base_address())[index];
}

descriptor64_t& table64_t::operator[](size_t index) {
    return static_cast<descriptor64_t*>(base_address())[index];
}

const descriptor64_t& table64_t::operator[](interrupt_t interrupt) const {
    return static_cast<const descriptor64_t*>(base_address())[static_cast<uint16_t>(interrupt)];
}

descriptor64_t& table64_t::operator[](interrupt_t interrupt) {
    return static_cast<descriptor64_t*>(base_address())[static_cast<uint16_t>(interrupt)];
}

}
