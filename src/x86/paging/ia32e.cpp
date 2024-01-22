
#include "x86/paging/ia32e.h"


namespace x86::paging::ia32e {

physical_address_t pml4e_t::address() const noexcept {
    return static_cast<physical_address_t>(bits.address) << page_bits_4k;
}

void pml4e_t::address(physical_address_t address) noexcept {
    auto maxphysaddr = max_physical_address_width();
    physical_address_t mask = (1ull << maxphysaddr) - 1;

    bits.address = (address >> page_bits_4k) & mask;
}

bool pdpte_t::is_huge() const noexcept {
    return huge.ps == 1;
}

physical_address_t pdpte_t::address() const noexcept {
    if (is_huge()) {
        return static_cast<physical_address_t>(huge.address) << page_bits_1g;
    } else {
        return static_cast<physical_address_t>(small.address) << page_bits_4k;
    }
}

void pdpte_t::address(physical_address_t address) noexcept {
    if (is_huge()) {
        auto maxphysaddr = max_physical_address_width();
        physical_address_t mask = (1ull << maxphysaddr) - 1;

        huge.address = (address >> page_bits_1g) & mask;
    } else {
        auto maxphysaddr = max_physical_address_width();
        physical_address_t mask = (1ull << maxphysaddr) - 1;

        small.address = (address >> page_bits_4k) & mask;
    }
}

bool pde_t::is_large() const noexcept {
    return large.ps == 1;
}

physical_address_t pde_t::address() const noexcept {
    if (is_large()) {
        return static_cast<physical_address_t>(large.address) << page_bits_2m;
    } else {
        return static_cast<physical_address_t>(small.address) << page_bits_4k;
    }
}

void pde_t::address(physical_address_t address) noexcept {
    if (is_large()) {
        auto maxphysaddr = max_physical_address_width();
        physical_address_t mask = (1ull << maxphysaddr) - 1;

        large.address = (address >> page_bits_2m) & mask;
    } else {
        auto maxphysaddr = max_physical_address_width();
        physical_address_t mask = (1ull << maxphysaddr) - 1;

        small.address = (address >> page_bits_4k) & mask;
    }
}

physical_address_t pte_t::address() const noexcept {
    return static_cast<physical_address_t>(bits.address) << page_bits_4k;
}

void pte_t::address(physical_address_t address) noexcept {
    auto maxphysaddr = max_physical_address_width();
    physical_address_t mask = (1ull << maxphysaddr) - 1;

    bits.address = (address >> page_bits_4k) & mask;
}

bool to_physical(x86::cr3_t& cr3, linear_address_t address, physical_address_t& out) noexcept {
    auto pml4_address = static_cast<physical_address_t>(cr3.ia32e.address) << page_bits_4k;
    auto pml4 = reinterpret_cast<const pml4e_t*>(pml4_address);
    auto& pml4e = pml4[address.huge.pml4e];
    if (!pml4e.bits.present) {
        return false;
    }

    auto pdpte_address = pml4e.address() | (static_cast<physical_address_t>(address.huge.directory_pointer) << 3);
    auto pdpte = reinterpret_cast<const pdpte_t*>(pdpte_address);
    if (!pdpte->huge.present) {
        return false;
    }

    if (pdpte->is_huge()) {
        out = pdpte->address() | static_cast<physical_address_t>(address.huge.offset);
        return true;
    }

    auto pde_address = pdpte->address() | (static_cast<physical_address_t>(address.large.directory) << 3);
    auto pde = reinterpret_cast<const pde_t*>(pde_address);
    if (!pde->large.present) {
        return false;
    }

    if (pde->is_large()) {
        out = pde->address() | static_cast<physical_address_t>(address.large.offset);
        return true;
    }

    auto pte_address = pde->address() | (static_cast<physical_address_t>(address.small.table) << 3);
    auto pte = reinterpret_cast<const pte_t*>(pte_address);
    if (!pte->bits.present) {
        return false;
    }

    out = pte->address() | static_cast<physical_address_t>(address.small.offset);
    return true;
}

}
