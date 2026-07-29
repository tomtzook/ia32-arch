
#include "x86/cpuid.h"
#include "x86/paging/paging.h"
#include "x86/paging/ia32e.h"


namespace x86::paging::ia32e {

physical_address_t pml4e_t::address() const {
    return static_cast<physical_address_t>(bits.pfn) << page_bits_4k;
}

void pml4e_t::address(const physical_address_t address) {
    const auto max_physical_address = max_physical_address_width();
    const physical_address_t mask = (1ull << max_physical_address) - 1;

    bits.pfn = (address >> page_bits_4k) & mask;
}

bool pdpte_t::is_huge() const {
    return huge.ps == 1;
}

physical_address_t pdpte_t::address() const {
    if (is_huge()) {
        return static_cast<physical_address_t>(huge.pfn) << page_bits_1g;
    }

    return static_cast<physical_address_t>(small.pfn) << page_bits_4k;
}

void pdpte_t::address(const physical_address_t address) {
    if (is_huge()) {
        const auto max_physical_address = max_physical_address_width();
        const physical_address_t mask = (1ull << max_physical_address) - 1;

        huge.pfn = (address >> page_bits_1g) & mask;
    } else {
        const auto max_physical_address = max_physical_address_width();
        const physical_address_t mask = (1ull << max_physical_address) - 1;

        small.pfn = (address >> page_bits_4k) & mask;
    }
}

bool pde_t::is_large() const {
    return large.ps == 1;
}

physical_address_t pde_t::address() const {
    if (is_large()) {
        return static_cast<physical_address_t>(large.pfn) << page_bits_2m;
    }

    return static_cast<physical_address_t>(small.pfn) << page_bits_4k;
}

void pde_t::address(const physical_address_t address) {
    if (is_large()) {
        const auto max_physical_address = max_physical_address_width();
        const physical_address_t mask = (1ull << max_physical_address) - 1;

        large.pfn = (address >> page_bits_2m) & mask;
    } else {
        const auto max_physical_address = max_physical_address_width();
        const physical_address_t mask = (1ull << max_physical_address) - 1;

        small.pfn = (address >> page_bits_4k) & mask;
    }
}

physical_address_t pte_t::address() const {
    return static_cast<physical_address_t>(bits.pfn) << page_bits_4k;
}

void pte_t::address(const physical_address_t address) {
    const auto max_physical_address = max_physical_address_width();
    const physical_address_t mask = (1ull << max_physical_address) - 1;

    bits.pfn = (address >> page_bits_4k) & mask;
}

bool are_huge_tables_supported() {
    // CPUID[0x80000001].EDX[26] = 1 -> 1gb pages supported [SDM 3 4.1.4 P109]
    const auto regs = x86::cpuid<cpuid_extended_processor_info_t>();
    return regs.edx.bits.page1gb != 0;
}

bool to_physical(const pml4e_t* pml4, const linear_address_t address, physical_address_t& out, to_virtual to_virtual) {
    const auto& pml4e = pml4[address.huge.pml4e];
    if (!pml4e.bits.present) {
        return false;
    }

    const auto pdpt_address = pml4e.address();
    const auto* pdpt = static_cast<const pdpte_t*>(to_virtual != nullptr ? to_virtual(pdpt_address) : reinterpret_cast<void*>(pdpt_address));
    const auto& pdpte = pdpt[address.huge.directory_pointer];
    if (!pdpte.huge.present) {
        return false;
    }

    if (pdpte.is_huge()) {
        out = pdpte.address() | static_cast<physical_address_t>(address.huge.offset);
        return true;
    }

    const auto pd_address = pdpte.address();
    const auto* pd = static_cast<const pde_t*>(to_virtual != nullptr ? to_virtual(pd_address) : reinterpret_cast<void*>(pd_address));
    const auto& pde = pd[address.large.directory];
    if (!pde.large.present) {
        return false;
    }

    if (pde.is_large()) {
        out = pde.address() | static_cast<physical_address_t>(address.large.offset);
        return true;
    }

    const auto pt_address = pde.address();
    const auto* pt = static_cast<const pte_t*>(to_virtual != nullptr ? to_virtual(pt_address) : reinterpret_cast<void*>(pt_address));
    const auto& pte = pt[address.small.table];
    if (!pte.bits.present) {
        return false;
    }

    out = pte.address() | static_cast<physical_address_t>(address.small.offset);
    return true;
}

bool to_physical(const cr3_t& cr3, const linear_address_t address, physical_address_t& out, const to_virtual to_virtual) {
    const auto pml4_address = static_cast<physical_address_t>(cr3.ia32e.address) << page_bits_4k;
    const auto pml4 = static_cast<const pml4e_t*>(to_virtual != nullptr ? to_virtual(pml4_address) : reinterpret_cast<void*>(pml4_address));
    return to_physical(pml4, address, out, to_virtual);
}

bool has_permissions(const pml4e_t* pml4, const linear_address_t address, const bool write, const bool execute, const to_virtual to_virtual) {
    const auto& pml4e = pml4[address.huge.pml4e];
    if (!pml4e.bits.present) {
        return false;
    }

    if (write && !pml4e.bits.rw) {
        return false;
    }
    if (execute && pml4e.bits.xd) {
        return false;
    }

    const auto pdpt_address = pml4e.address();
    const auto* pdpt = static_cast<const pdpte_t*>(to_virtual != nullptr ? to_virtual(pdpt_address) : reinterpret_cast<void*>(pdpt_address));
    const auto& pdpte = pdpt[address.huge.directory_pointer];
    if (!pdpte.huge.present) {
        return false;
    }

    if (pdpte.is_huge()) {
        if (write && !pdpte.huge.rw) {
            return false;
        }
        if (execute && pdpte.huge.xd) {
            return false;
        }

        return true;
    }

    if (write && !pdpte.small.rw) {
        return false;
    }
    if (execute && pdpte.small.xd) {
        return false;
    }

    const auto pd_address = pdpte.address();
    const auto* pd = static_cast<const pde_t*>(to_virtual != nullptr ? to_virtual(pd_address) : reinterpret_cast<void*>(pd_address));
    const auto& pde = pd[address.large.directory];
    if (!pde.large.present) {
        return false;
    }

    if (pde.is_large()) {
        if (write && !pde.large.rw) {
            return false;
        }
        if (execute && pde.large.xd) {
            return false;
        }

        return true;
    }

    if (write && !pde.small.rw) {
        return false;
    }
    if (execute && pde.small.xd) {
        return false;
    }

    const auto pt_address = pde.address();
    const auto* pt = static_cast<const pte_t*>(to_virtual != nullptr ? to_virtual(pt_address) : reinterpret_cast<void*>(pt_address));
    const auto& pte = pt[address.small.table];
    if (!pte.bits.present) {
        return false;
    }

    if (write && !pte.bits.rw) {
        return false;
    }
    if (execute && pte.bits.xd) {
        return false;
    }

    return true;
}

bool has_permissions(const cr3_t& cr3, linear_address_t address, const bool write, const bool execute, const to_virtual to_virtual) {
    const auto pml4_address = static_cast<physical_address_t>(cr3.ia32e.address) << page_bits_4k;
    const auto pml4 = static_cast<const pml4e_t*>(to_virtual != nullptr ? to_virtual(pml4_address) : reinterpret_cast<void*>(pml4_address));
    return has_permissions(pml4, address, write, execute, to_virtual);
}

bool apply_permissions(pml4e_t* pml4, const linear_address_t address, const bool read_write, const bool execute, const to_virtual to_virtual) {
    auto& pml4e = pml4[address.huge.pml4e];
    if (!pml4e.bits.present) {
        return false;
    }

    if (read_write) {
        pml4e.bits.rw = true;
    }
    if (execute) {
        pml4e.bits.xd = false;
    }

    const auto pdpt_address = pml4e.address();
    auto* pdpt = static_cast<pdpte_t*>(to_virtual != nullptr ? to_virtual(pdpt_address) : reinterpret_cast<void*>(pdpt_address));
    auto& pdpte = pdpt[address.huge.directory_pointer];
    if (!pdpte.huge.present) {
        return false;
    }

    if (pdpte.is_huge()) {
        pdpte.huge.rw = read_write;
        pdpte.huge.xd = !execute;
        return true;
    }

    if (read_write) {
        pdpte.small.rw = true;
    }
    if (execute) {
        pdpte.small.xd = false;
    }

    const auto pd_address = pdpte.address();
    auto* pd = static_cast<pde_t*>(to_virtual != nullptr ? to_virtual(pd_address) : reinterpret_cast<void*>(pd_address));
    auto& pde = pd[address.large.directory];
    if (!pde.large.present) {
        return false;
    }

    if (pde.is_large()) {
        pde.large.rw = read_write;
        pde.large.xd = !execute;
        return true;
    }

    if (read_write) {
        pde.small.rw = true;
    }
    if (execute) {
        pde.small.xd = false;
    }

    const auto pt_address = pde.address();
    auto* pt = static_cast<pte_t*>(to_virtual != nullptr ? to_virtual(pt_address) : reinterpret_cast<void*>(pt_address));
    auto& pte = pt[address.small.table];
    if (!pte.bits.present) {
        return false;
    }

    pte.bits.rw = read_write;
    pte.bits.xd = !execute;
    return true;
}

bool apply_permissions(const cr3_t& cr3, const linear_address_t address, const bool read_write, const bool execute, const to_virtual to_virtual) {
    const auto pml4_address = static_cast<physical_address_t>(cr3.ia32e.address) << page_bits_4k;
    const auto pml4 = static_cast<pml4e_t*>(to_virtual != nullptr ? to_virtual(pml4_address) : reinterpret_cast<void*>(pml4_address));
    return apply_permissions(pml4, address, read_write, execute, to_virtual);
}

}
