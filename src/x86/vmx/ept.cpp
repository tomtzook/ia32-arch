
#include "x86/paging/paging.h"
#include "x86/vmx/ept.h"


namespace x86::vmx {

bool pml4e_t::present() const {
    return bits.read | bits.write | bits.execute;
}

physical_address_t pml4e_t::address() const {
    return static_cast<physical_address_t>(bits.pfn) << paging::page_bits_4k;
}

void pml4e_t::address(const physical_address_t address) {
    const auto max_physical_address = paging::max_physical_address_width();
    const physical_address_t mask = (1ull << max_physical_address) - 1;

    bits.pfn = (address >> paging::page_bits_4k) & mask;
}

bool pdpte_t::present() const {
    return huge.read | huge.write | huge.execute;
}

bool pdpte_t::is_huge() const {
    return huge.ps == 1;
}

physical_address_t pdpte_t::address() const {
    if (is_huge()) {
        return static_cast<physical_address_t>(huge.pfn) << paging::page_bits_1g;
    }

    return static_cast<physical_address_t>(small.pfn) << paging::page_bits_4k;
}

void pdpte_t::address(const physical_address_t address) {
    if (is_huge()) {
        const auto max_physical_address = paging::max_physical_address_width();
        const physical_address_t mask = (1ull << max_physical_address) - 1;

        huge.pfn = (address >> paging::page_bits_1g) & mask;
    } else {
        const auto max_physical_address = paging::max_physical_address_width();
        const physical_address_t mask = (1ull << max_physical_address) - 1;

        small.pfn = (address >> paging::page_bits_4k) & mask;
    }
}

bool pde_t::present() const {
    return large.read | large.write | large.execute;
}

bool pde_t::is_large() const {
    return large.ps == 1;
}

physical_address_t pde_t::address() const {
    if (is_large()) {
        return static_cast<physical_address_t>(large.pfn) << paging::page_bits_2m;
    }

    return static_cast<physical_address_t>(small.pfn) << paging::page_bits_4k;
}

void pde_t::address(const physical_address_t address) {
    if (is_large()) {
        const auto max_physical_address = paging::max_physical_address_width();
        const physical_address_t mask = (1ull << max_physical_address) - 1;

        large.pfn = (address >> paging::page_bits_2m) & mask;
    } else {
        const auto max_physical_address = paging::max_physical_address_width();
        const physical_address_t mask = (1ull << max_physical_address) - 1;

        small.pfn = (address >> paging::page_bits_4k) & mask;
    }
}

bool pte_t::present() const {
    return bits.read | bits.write | bits.execute;
}

physical_address_t pte_t::address() const {
    return static_cast<physical_address_t>(bits.pfn) << paging::page_bits_4k;
}

void pte_t::address(const physical_address_t address) {
    const auto max_physical_address = paging::max_physical_address_width();
    const physical_address_t mask = (1ull << max_physical_address) - 1;

    bits.pfn = (address >> paging::page_bits_4k) & mask;
}

physical_address_t ept_pointer_t::address() const {
    return static_cast<physical_address_t>(bits.address) << paging::page_bits_4k;
}

void ept_pointer_t::address(const physical_address_t address) {
    const auto max_physical_address = paging::max_physical_address_width();
    const physical_address_t mask = (1ull << max_physical_address) - 1;

    bits.address = (address >> paging::page_bits_4k) & mask;
}

bool to_physical(const ept_pointer_t& eptp, const guest_physical_address_t address, physical_address_t& out) {
    const auto pml4_address = static_cast<physical_address_t>(eptp.bits.address) << paging::page_bits_4k;
    const auto pml4 = reinterpret_cast<const pml4e_t*>(pml4_address);
    auto& pml4e = pml4[address.huge.pml4e];
    if (!pml4e.present()) {
        return false;
    }

    const auto pdpte_address = pml4e.address() | (static_cast<physical_address_t>(address.huge.directory_pointer) << 3);
    const auto pdpte = reinterpret_cast<const pdpte_t*>(pdpte_address);
    if (!pdpte->present()) {
        return false;
    }

    if (pdpte->is_huge()) {
        out = pdpte->address() | static_cast<physical_address_t>(address.huge.offset);
        return true;
    }

    const auto pde_address = pdpte->address() | (static_cast<physical_address_t>(address.large.directory) << 3);
    const auto pde = reinterpret_cast<const pde_t*>(pde_address);
    if (!pde->present()) {
        return false;
    }

    if (pde->is_large()) {
        out = pde->address() | static_cast<physical_address_t>(address.large.offset);
        return true;
    }

    const auto pte_address = pde->address() | (static_cast<physical_address_t>(address.small.table) << 3);
    const auto pte = reinterpret_cast<const pte_t*>(pte_address);
    if (!pte->present()) {
        return false;
    }

    out = pte->address() | static_cast<physical_address_t>(address.small.offset);
    return true;
}

}
