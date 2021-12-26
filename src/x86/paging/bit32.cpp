#include "x86/cr.h"
#include "x86/cpuid.h"
#include "x86/paging/bit32.h"


namespace x86::paging::bit32 {

bool pde_t::is_big() const noexcept {
    // assuming it's supported and enabled
    return big.ps == 1;
}

physical_address_t pde_t::address() const noexcept {
    if (is_big()) {
        return (static_cast<physical_address_t>(big.address2) << page_bits_4m) | pse36_address_bits();
    } else {
        return static_cast<physical_address_t>(small.address) << page_bits_4k;
    }
}

void pde_t::address(physical_address_t address) noexcept {
    if (is_big()) {
        big.address2 = (address >> page_bits_4m) & 0x3ff;
        pse36_address_bits(address);
    } else {
        small.address = static_cast<uint32_t>(address >> page_bits_4k);
    }
}

physical_address_t pde_t::pse36_address_bits() const noexcept {
    if (!is_pse36_supported()) {
        return 0;
    }

    auto maxphysaddr = max_physical_address_width();
    physical_address_t mask = ((maxphysaddr - 20) - 13 + 1) << 13;
    return (big.address2 & ~(1ull << mask)) << 32;
}

void pde_t::pse36_address_bits(physical_address_t address) noexcept {
    if (!is_pse36_supported()) {
        return;
    }

    auto maxphysaddr = max_physical_address_width();
    physical_address_t mask = ((maxphysaddr - 20) - 13 + 1) << 13;
    big.address = (address >> 32) & ~(1ull << mask);
}

physical_address_t pte_t::address() const noexcept {
    return static_cast<physical_address_t>(bits.address) << page_bits_4k;
}

void pte_t::address(physical_address_t address) noexcept {
    bits.address = static_cast<uint32_t>(address >> page_bits_4k);
}

bool are_4m_page_tables_supported() noexcept {
    // [SDM 3 4.1.4 P109]
    auto cpuid = x86::cpuid<cpuid_eax01_t>();
    return cpuid.edx.bits.pse != 0;
}

bool are_4m_page_tables_enabled() noexcept {
    // [SDM 3 4.1.4 P109]
    if (!are_4m_page_tables_supported()) {
        return false;
    }

    auto cr4 = read<x86::cr4_t>();
    return cr4.bits.page_size_extensions != 0;
}

bool is_pse36_supported() noexcept {
    // [SDM 3 4.1.4 P109]
    auto cpuid = x86::cpuid<cpuid_eax01_t>();
    return cpuid.edx.bits.pse36 != 0;
}

bool to_physical(x86::cr3_t& cr3, linear_address_t address, physical_address_t& out) noexcept {
    auto pde_address = cr3.address() | (address.big.directory << 2);
    auto pde = reinterpret_cast<const x86::paging::bit32::pde_t*>(pde_address);
    if (!pde->is_present()) {
        return false;
    }

    if (pde->is_big()) { // assuming that CR4.PSE is actually enabled
        out = pde->address() | (address.big.offset);
        return true;
    } else {
        auto pte_address = pde->address() | (address.small.table << 2);
        auto pte = reinterpret_cast<const x86::paging::bit32::pte_t*>(pte_address);
        if (!pte->is_present()) {
            return false;
        }

        out = pte->address() | (address.small.offset);
        return true;
    }
}

}
