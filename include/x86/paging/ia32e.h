#pragma once

#include "x86/common.h"
#include "x86/paging/paging.h"
#include "x86/cr.h"

namespace x86::paging::ia32e {

// IA-32E Paging [SDM 3 4.5 P123]
// translation:
// CR3 -> address to PML4E
// - Bits 51:12 from CR3.[51:12].
// - Bits 11:3 are bits 47:39 of linear address.
// - Bits 2:0 are 0
// PML4E -> address to PDPTE
// - Bits 51:12 from PML4E.
// - Bits 11:3 are bits 38:30 of linear address.
// - Bits 2:0 are 0
// If PDPTE.PS is 1 = maps to 1G page
// - Bits 51:30 are from the PDPTE.
// - Bits 29:0 are from the original linear address.
// If PDPTE.PS is 0 = maps to PDE
// - Bits 51:12 from PDPTE.
// - Bits 11:3 are bits 29:21 of linear address.
// - Bits 2:0 are 0
// If PDE.PS is 1 = maps to 2M page
// - Bits 51:21 are from the PDE.
// - Bits 20:0 are from the original linear address.
// If PDE.PS is 0 = maps to PTE
// - Bits 51:12 are from the PDE.
// - Bits 11:3 are bits 20:12 of the linear address.
// - Bits 2:0 are 0.
// PTE -> 4K page
// - Bits 51:12 are from the PTE.
// - Bits 11:0 are from the original linear address.

static constexpr size_t pml4e_in_pml4 = 512;
static constexpr size_t pdptes_in_pdpt = 512;
static constexpr size_t pdes_in_directory = 512;
static constexpr size_t ptes_in_table = 512;

#pragma pack(push, 1)

// [SDM 3 4.5 P124 "Figure 4-8"/"Figure 4-9"/"Figure 4-10"]
struct linear_address_t {
    union {
        struct { // maps to 4k page
            uint64_t offset : 12;
            uint64_t table : 9;
            uint64_t directory : 9;
            uint64_t directory_pointer : 9;
            uint64_t pml4e : 9;
        } small;
        struct { // maps to 2m page
            uint64_t offset : 21;
            uint64_t directory : 9;
            uint64_t directory_pointer : 9;
            uint64_t pml4e : 9;
        } large;
        struct { // maps to 1g page
            uint64_t offset : 30;
            uint64_t directory_pointer : 9;
            uint64_t pml4e : 9;
        } huge;
        uint64_t raw;
    };
};
static_assert(sizeof(linear_address_t) == 8, "sizeof(linear_address_t)");

// [SDM 3 4.5 P127 "Table 4-14"]
struct pml4e_t {
    union {
        struct {
            uint64_t present : 1;
            uint64_t rw : 1;
            uint64_t us : 1;
            uint64_t pwt : 1;
            uint64_t pcd : 1;
            uint64_t accessed : 1;
            uint64_t ignored0 : 1;
            uint64_t reserved0 : 1;
            uint64_t ignored1 : 4;
            uint64_t address : 40; // [12:51] depends on maxphysaddr
            uint64_t ignored2 : 11;
            uint64_t xd : 1;
        } bits;
        uint64_t raw;
    };

    physical_address_t address() const;
    void address(physical_address_t address);
};
static_assert(sizeof(pml4e_t) == 8, "sizeof(pml4e_t)");

// [SDM 3 4.5 P128 "Table 4-15"/"Table 4-16"]
struct pdpte_t {
    union {
        struct {
            uint64_t present : 1;
            uint64_t rw : 1;
            uint64_t us : 1;
            uint64_t pwt : 1;
            uint64_t pcd : 1;
            uint64_t accessed : 1;
            uint64_t ignored0 : 1;
            uint64_t ps : 1;
            uint64_t ignored1 : 4;
            uint64_t address : 40; // [12:51] depends on maxphysaddr
            uint64_t ignored2 : 11;
            uint64_t xd : 1;
        } small;
        struct {
            uint64_t present : 1;
            uint64_t rw : 1;
            uint64_t us : 1;
            uint64_t pwt : 1;
            uint64_t pcd : 1;
            uint64_t accessed : 1;
            uint64_t dirty : 1;
            uint64_t ps : 1;
            uint64_t global : 1;
            uint64_t ignored0 : 3;
            uint64_t pat : 1;
            uint64_t reserved0 : 17;
            uint64_t address : 22; // [30:51] depends on maxphysaddr
            uint64_t ignored1 : 7;
            uint64_t protection_key : 4;
            uint64_t xd : 1;
        } huge;
        uint64_t raw;
    };

    bool is_huge() const;

    physical_address_t address() const;
    void address(physical_address_t address);
};
static_assert(sizeof(pdpte_t) == 8, "sizeof(pdpte_t)");

// [SDM 3 4.5 P129 "Table 4-17"/"Table 4-18"]
struct pde_t {
    union {
        struct {
            uint64_t present : 1;
            uint64_t rw : 1;
            uint64_t us : 1;
            uint64_t pwt : 1;
            uint64_t pcd : 1;
            uint64_t accessed : 1;
            uint64_t ignored0 : 1;
            uint64_t ps : 1;
            uint64_t ignored1 : 4;
            uint64_t address : 40; // [12:51] depends on maxphysaddr
            uint64_t ignored2 : 11;
            uint64_t xd : 1;
        } small;
        struct {
            uint64_t present : 1;
            uint64_t rw : 1;
            uint64_t us : 1;
            uint64_t pwt : 1;
            uint64_t pcd : 1;
            uint64_t accessed : 1;
            uint64_t dirty : 1;
            uint64_t ps : 1;
            uint64_t global : 1;
            uint64_t ignored0 : 3;
            uint64_t pat : 1;
            uint64_t reserved0 : 8;
            uint64_t address : 31; // [21:51] depends on maxphysaddr
            uint64_t ignored1 : 7;
            uint64_t protection_key : 4;
            uint64_t xd : 1;
        } large;
        uint64_t raw;
    };

    bool is_large() const;

    physical_address_t address() const;
    void address(physical_address_t address);
};
static_assert(sizeof(pde_t) == 8, "sizeof(pde_t)");

// [SDM 3 4.5 P131 "Table 4-19"]
struct pte_t {
    union {
        struct {
            uint64_t present : 1;
            uint64_t rw : 1;
            uint64_t us : 1;
            uint64_t pwt : 1;
            uint64_t pcd : 1;
            uint64_t accessed : 1;
            uint64_t dirty : 1;
            uint64_t pat : 1;
            uint64_t global : 1;
            uint64_t ignored0 : 3;
            uint64_t address : 40; // [12:51] depends on maxphysaddr
            uint64_t ignored1 : 7;
            uint64_t protection_key : 4;
            uint64_t xd : 1;
        } bits;
        uint64_t raw;
    };

    physical_address_t address() const;
    void address(physical_address_t address);
};
static_assert(sizeof(pte_t) == 8, "sizeof(pte_t)");

#pragma pack(pop)

bool are_huge_tables_supported();

bool to_physical(x86::cr3_t& cr3, linear_address_t address, physical_address_t& out);

}
