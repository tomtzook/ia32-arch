
#include "x86/segments.h"


namespace x86::segments {

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

table_t::table_t(table_register_t table_register) noexcept
    : m_table_register(table_register) {
}

const void* table_t::base_address() const noexcept {
    return reinterpret_cast<const void*>(m_table_register.base_address);
}

void* table_t::base_address() noexcept {
    return reinterpret_cast<void*>(m_table_register.base_address);
}

size_t table_t::limit() const noexcept {
    return m_table_register.limit;
}

size_t table_t::count() const noexcept {
    return static_cast<size_t>(limit() + 1) / sizeof(x86::segments::descriptor_t);
}

const descriptor_t& table_t::operator[](size_t index) const noexcept {
    return reinterpret_cast<const descriptor_t*>(base_address())[index];
}

descriptor_t& table_t::operator[](size_t index) noexcept {
    return reinterpret_cast<descriptor_t*>(base_address())[index];
}

const descriptor_t& table_t::operator[](const selector_t& selector) const noexcept {
    return this->operator[](selector.bits.index);
}

descriptor_t& table_t::operator[](const selector_t& selector) noexcept {
    return this->operator[](selector.bits.index);
}

}
