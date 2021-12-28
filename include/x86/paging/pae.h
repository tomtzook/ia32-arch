#pragma once

#include "x86/common.h"
#include "x86/paging/paging.h"
#include "x86/cr.h"


namespace x86::paging::pae {

// PAE Paging [SDM 3 4.4 P117]

static constexpr size_t pdpte_registers = 4;
static constexpr size_t pdes_in_directory = 512;
static constexpr size_t ptes_in_table = 512;

#pragma pack(push, 1)

// [SDM 3 4.4.2 P119 "Figure 4-5"/"Figure 4-6"]
struct linear_address_t {
    union {
        struct { // maps to 4k page
            uint32_t offset : 12;
            uint32_t table : 9;
            uint32_t directory : 9;
            uint32_t directory_pointer : 2;
        } small;
        struct { // maps to 2m page
            uint32_t offset : 21;
            uint32_t directory : 9;
            uint32_t directory_pointer : 2;
        } big;
        uint32_t raw;
    };
};
static_assert(sizeof(linear_address_t) == 4, "sizeof(linear_address_t)");

// [SDM 3 4.4.1 P118 "Table 4-8"]
struct pdpte_t {
    union {
        struct {
            uint64_t present : 1;
            uint64_t reserved0 : 1;
            uint64_t pwt : 1;
            uint64_t pcd : 1;
            uint64_t reserved1 : 4;
            uint64_t ignored0 : 3;
            uint64_t address : 52; // [12:63] depends on maxphysaddr
        } bits;
        uint64_t raw;
    };

    physical_address_t address() const noexcept;
    void address(physical_address_t address) noexcept;
};
static_assert(sizeof(pdpte_t) == 8, "sizeof(pdpte_t)");

// [SDM 3 4.4.2 P120 "Table 4-9"/"Table 4-10"]
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
            uint64_t address : 51; // [12:62] depends on maxphysaddr
            uint64_t nx : 1;
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
            uint64_t address : 42; // [21:62] depends on maxphysaddr
            uint64_t nx : 1;
        } big;
        uint64_t raw;
    };

    bool is_big() const noexcept;

    physical_address_t address() const noexcept;
    void address(physical_address_t address) noexcept;
};
static_assert(sizeof(pde_t) == 8, "sizeof(pde_t)");

// [SDM 3 4.4.2 P121 "Table 4-11"]
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
            uint64_t address : 51; // [12:62] depends on maxphysaddr
            uint64_t nx : 1;
        } bits;
        uint64_t raw;
    };

    physical_address_t address() const noexcept;
    void address(physical_address_t address) noexcept;
};
static_assert(sizeof(pte_t) == 8, "sizeof(pte_t)");

#pragma pack(pop)

bool to_physical(x86::cr3_t& cr3, linear_address_t address, physical_address_t& out) noexcept;

}
