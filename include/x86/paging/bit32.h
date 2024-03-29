#pragma once

#include "x86/common.h"
#include "x86/paging/paging.h"
#include "x86/cr.h"


namespace x86::paging::bit32 {

// 32 bit Paging [SDM 3 4.3 P111]
// translation:
// CR3 -> address to PDE
//  - Bits 39:32 are all 0.
//  - Bits 31:12 are from CR3.
//  - Bits 11:2 are bits 31:22 of the linear address.
//  - Bits 1:0 are 0.
// If CR4.PSE = 1 and the PDE’s PS flag is 1 = maps to 4M page.
// - Bits 39:32 are bits 20:13 of the PDE. [affected by PSE36]
// - Bits 31:22 are bits 31:22 of the PDE.
// — Bits 21:0 are from the original linear address.
// If CR4.PSE = 0 or the PDE’s PS flag is 0 = maps to PTE
// - Bits 39:32 are all 0.
// - Bits 31:12 are from the PDE.
// - Bits 11:2 are bits 21:12 of the linear address.
// - Bits 1:0 are 0.
// PTE -> 4K page
// - Bits 39:32 are all 0.
// - Bits 31:12 are from the PTE.
// - Bits 11:0 are from the original linear address.

/*
 * The following code shows an example of a page table
 * which uses 4M pages and a single page directory

    #define page_aligned __attribute__ ((aligned(4096)))

    struct page_table_t {
        x86::paging::bit32::pde_t pdes[x86::paging::bit32::pdes_in_directory] page_aligned;
    } page_aligned;

 * It is then possible to initialize paging with it.
 * For example, setting up identity paging:

    memset(&page_table, 0, sizeof(page_table_t));

    for (size_t i = 0; i < array_size(page_table.pdes); ++i) {
        auto& pde = page_table.pdes[i];
        pde.big.present = true;
        pde.big.rw = true;
        pde.big.ps = true;

        pde.address(i * x86::paging::page_size_4m);
    }

 * And finally, paging must be set up and enabled.
 * Assuming system is in protected mode:

    auto cr3 = x86::read<x86::cr3_t>();
    cr3.bit32.address = reinterpret_cast<physical_address_t>(&page_table) >> 12;
    x86::write(cr3);

    auto cr4 = x86::read<x86::cr4_t>();
    cr4.bits.page_size_extensions = true; // only necessary for 4M pages
    x86::write(cr4);

    auto cr0 = x86::read<x86::cr0_t>();
    cr0.bits.paging_enable = true;
    x86::write(cr0);
 */

static constexpr size_t pdes_in_directory = 1024;
static constexpr size_t ptes_in_table = 1024;

#pragma pack(push, 1)

// [SDM 3 4.3 P113 "Figure 4-2"/"Figure 4-3"]
struct linear_address_t {
    union {
        struct { // maps to 4k page
            uint32_t offset : 12;
            uint32_t table : 10;
            uint32_t directory : 10;
        } small;
        struct { // maps to 4m page
            uint32_t offset : 22;
            uint32_t directory : 10;
        } big;
        uint32_t raw;
    };
};
static_assert(sizeof(linear_address_t) == 4, "sizeof(linear_address_t)");

// [SDM 3 4.3 P115 "Table 4-4/Table 4-5"]
struct pde_t {
    union {
        struct { // maps to 4k page
            uint32_t present : 1;
            uint32_t rw : 1;
            uint32_t us : 1;
            uint32_t pwt : 1;
            uint32_t pcd : 1;
            uint32_t accessed : 1;
            uint32_t ignored0 : 1;
            uint32_t ps : 1;
            uint32_t ignored1 : 3;
            uint32_t address : 20;
        } small;
        struct { // maps to 4m page
            uint32_t present : 1;
            uint32_t rw : 1;
            uint32_t us : 1;
            uint32_t pwt : 1;
            uint32_t pcd : 1;
            uint32_t accessed : 1;
            uint32_t dirty : 1;
            uint32_t ps : 1;
            uint32_t global : 1;
            uint32_t ignored0 : 3;
            uint32_t pat : 1;
            // [SDM 3 4.3 P115 "Table 4-4 Notes 2."]
            uint32_t address : 8; // [21:13] affected by PSE36
            uint32_t reserved0 : 1;
            uint32_t address2 : 10; // [31:22]
        } big;
        uint32_t raw;
    };

    bool is_present() const noexcept { return small.present; }
    bool is_big() const noexcept;

    physical_address_t address() const noexcept;
    void address(physical_address_t address) noexcept;
    physical_address_t pse36_address_bits() const noexcept;
    void pse36_address_bits(physical_address_t address) noexcept;
};
static_assert(sizeof(pde_t) == 4, "sizeof(pde_t)");

// [SDM 3 4.3 P115 "Table 4-6"]
struct pte_t {
    union {
        struct {
            uint32_t present : 1;
            uint32_t rw : 1;
            uint32_t us : 1;
            uint32_t pwt : 1;
            uint32_t pcd : 1;
            uint32_t accessed : 1;
            uint32_t dirty : 1;
            uint32_t pat : 1;
            uint32_t global : 1;
            uint32_t ignored0 : 3;
            uint32_t address : 20;
        } bits;
        uint32_t raw;
    };

    bool is_present() const noexcept { return bits.present; }

    physical_address_t address() const noexcept;
    void address(physical_address_t address) noexcept;
};
static_assert(sizeof(pte_t) == 4, "sizeof(pte_t)");

#pragma pack(pop)

bool are_4m_page_tables_supported() noexcept;
bool are_4m_page_tables_enabled() noexcept;
bool is_pse36_supported() noexcept;

bool to_physical(x86::cr3_t& cr3, linear_address_t address, physical_address_t& out) noexcept;

}
