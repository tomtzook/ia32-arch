
#include "x86/paging/paging.h"
#include "x86/paging/pae.h"


namespace x86::paging::pae {

physical_address_t pdpte_t::address() const {
    return static_cast<physical_address_t>(bits.address) << page_bits_4k;
}

void pdpte_t::address(const physical_address_t address) {
    const auto max_physical_address = max_physical_address_width();
    const physical_address_t mask = (1ull << max_physical_address) - 1;

    bits.address = (address >> page_bits_4k) & mask;
}

bool pde_t::is_big() const {
    return big.ps == 1;
}

physical_address_t pde_t::address() const {
    if (is_big()) {
        return static_cast<physical_address_t>(big.address) << page_bits_2m;
    }

    return static_cast<physical_address_t>(small.address) << page_bits_4k;
}

void pde_t::address(const physical_address_t address) {
    if (is_big()) {
        const auto max_physical_address = max_physical_address_width();
        const physical_address_t mask = (1ull << max_physical_address) - 1;

        big.address = (address >> page_bits_2m) & mask;
    } else {
        const auto max_physical_address = max_physical_address_width();
        const physical_address_t mask = (1ull << max_physical_address) - 1;

        small.address = (address >> page_bits_4k) & mask;
    }
}

physical_address_t pte_t::address() const {
    return static_cast<physical_address_t>(bits.address) << page_bits_4k;
}

void pte_t::address(const physical_address_t address) {
    const auto max_physical_address = max_physical_address_width();
    const physical_address_t mask = (1ull << max_physical_address) - 1;

    bits.address = (address >> page_bits_4k) & mask;
}

bool to_physical(const cr3_t& cr3, const linear_address_t address, physical_address_t& out) {
    const auto pdpt_address = static_cast<physical_address_t>(cr3.pae.address) << 5;
    const auto ptpt = reinterpret_cast<const pdpte_t*>(pdpt_address);
    auto& ptpte = ptpt[address.big.directory_pointer];
    if (!ptpte.bits.present) {
        return false;
    }

    const auto pde_address = ptpte.address() | (static_cast<physical_address_t>(address.big.directory) << 3);
    const auto pde = reinterpret_cast<const pde_t*>(pde_address);
    if (!pde->big.present) {
        return false;
    }

    if (pde->is_big()) {
        out = pde->address() | static_cast<physical_address_t>(address.big.offset);
        return true;
    }

    const auto pte_address = pde->address() | (static_cast<physical_address_t>(address.small.table) << 3);
    const auto pte = reinterpret_cast<const pte_t*>(pte_address);
    if (!pte->bits.present) {
        return false;
    }

    out = pte->address() | static_cast<physical_address_t>(address.small.offset);
    return true;
}

}
