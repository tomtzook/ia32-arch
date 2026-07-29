
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

bool to_physical(const ept_pointer_t& eptp, const guest_physical_address_t address, physical_address_t& out, paging::to_virtual to_virtual) {
    const auto pml4_address = static_cast<physical_address_t>(eptp.bits.address) << paging::page_bits_4k;
    const auto pml4 = reinterpret_cast<const pml4e_t*>(pml4_address);
    auto& pml4e = pml4[address.huge.pml4e];
    if (!pml4e.present()) {
        return false;
    }

    const auto pdpt_address = pml4e.address();
    const auto* pdpt = static_cast<const pdpte_t*>(to_virtual != nullptr ? to_virtual(pdpt_address) : reinterpret_cast<void*>(pdpt_address));
    const auto& pdpte = pdpt[address.huge.directory_pointer];
    if (!pdpte.present()) {
        return false;
    }

    if (pdpte.is_huge()) {
        out = pdpte.address() | static_cast<physical_address_t>(address.huge.offset);
        return true;
    }

    const auto pd_address = pdpte.address();
    const auto* pd = static_cast<const pde_t*>(to_virtual != nullptr ? to_virtual(pd_address) : reinterpret_cast<void*>(pd_address));
    const auto& pde = pd[address.large.directory];
    if (!pde.present()) {
        return false;
    }

    if (pde.is_large()) {
        out = pde.address() | static_cast<physical_address_t>(address.large.offset);
        return true;
    }

    const auto pt_address = pde.address();
    const auto* pt = static_cast<const pte_t*>(to_virtual != nullptr ? to_virtual(pt_address) : reinterpret_cast<void*>(pt_address));
    const auto& pte = pt[address.small.table];
    if (!pte.present()) {
        return false;
    }

    out = pte.address() | static_cast<physical_address_t>(address.small.offset);
    return true;
}

bool apply_permissions(const ept_pointer_t& eptp, const guest_physical_address_t address, const bool read, const bool write, const bool execute, paging::to_virtual to_virtual) {
    const auto pml4_address = static_cast<physical_address_t>(eptp.bits.address) << paging::page_bits_4k;
    auto pml4 = reinterpret_cast<pml4e_t*>(pml4_address);
    auto& pml4e = pml4[address.huge.pml4e];
    if (!pml4e.present()) {
        return false;
    }

    pml4e.bits.read |= read;
    pml4e.bits.write |= write;
    pml4e.bits.execute |= execute;

    const auto pdpt_address = pml4e.address();
    auto* pdpt = static_cast<pdpte_t*>(to_virtual != nullptr ? to_virtual(pdpt_address) : reinterpret_cast<void*>(pdpt_address));
    auto& pdpte = pdpt[address.huge.directory_pointer];
    if (!pdpte.present()) {
        return false;
    }

    if (pdpte.is_huge()) {
        pdpte.huge.read = read;
        pdpte.huge.write = write;
        pdpte.huge.execute = execute;
        return true;
    }

    pdpte.small.read |= read;
    pdpte.small.write |= write;
    pdpte.small.execute |= execute;

    const auto pd_address = pdpte.address();
    auto* pd = static_cast<pde_t*>(to_virtual != nullptr ? to_virtual(pd_address) : reinterpret_cast<void*>(pd_address));
    auto& pde = pd[address.large.directory];
    if (!pde.present()) {
        return false;
    }

    if (pde.is_large()) {
        pde.large.read = read;
        pde.large.write = write;
        pde.large.execute = execute;
        return true;
    }

    pde.small.read |= read;
    pde.small.write |= write;
    pde.small.execute |= execute;

    const auto pt_address = pde.address();
    auto* pt = static_cast<pte_t*>(to_virtual != nullptr ? to_virtual(pt_address) : reinterpret_cast<void*>(pt_address));
    auto& pte = pt[address.small.table];
    if (!pte.present()) {
        return false;
    }

    pte.bits.read = read;
    pte.bits.write = write;
    pte.bits.execute = execute;
    return true;
}

}
