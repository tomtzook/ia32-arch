#pragma once

#include "x86/common.h"
#include "x86/vmx/error.h"


namespace x86::vmx {

// EPT [SDM 3 28.2 P1157]

static constexpr size_t pml4e_in_pml4 = 512;
static constexpr size_t pdptes_in_pdpt = 512;
static constexpr size_t pdes_in_directory = 512;
static constexpr size_t ptes_in_table = 512;

#pragma pack(push, 1)

struct guest_physical_address_t {
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
static_assert(sizeof(guest_physical_address_t) == 8, "sizeof(guest_physical_address_t)");

struct pml4e_t {
    union {
        struct {
            // [SDM 3 28.2.2 P1158 "Table 28-1"]
            uint64_t read : 1;
            uint64_t write : 1;
            uint64_t execute : 1;
            uint64_t reserved0 : 5;
            uint64_t accessed : 1;
            uint64_t ignored0 : 1;
            uint64_t user_mode_execute : 1;
            uint64_t ignored1 : 1;
            uint64_t address : 40; // [12:51] depends on maxphysaddr
            uint64_t ignored2 : 12;
        } bits;
        uint64_t raw;
    };

    bool present() const;
    physical_address_t address() const;
    void address(physical_address_t address);
};
static_assert(sizeof(pml4e_t) == 8, "sizeof(pml4e_t)");

struct pdpte_t {
    union {
        struct {
            // [SDM 3 28.2.2 P1162 "Table 28-3"]
            uint64_t read : 1;
            uint64_t write : 1;
            uint64_t execute : 1;
            uint64_t reserved0 : 5;
            uint64_t accessed : 1;
            uint64_t ignored0 : 1;
            uint64_t user_mode_execute : 1;
            uint64_t ignored1 : 1;
            uint64_t address : 40; // [12:51] depends on maxphysaddr
            uint64_t ignored2 : 12;
        } small;
        struct {
            // [SDM 3 28.2.2 P1161 "Table 28-2"]
            uint64_t read : 1;
            uint64_t write : 1;
            uint64_t execute : 1;
            uint64_t mem_type : 3;
            uint64_t ignore_pat : 1;
            uint64_t ps : 1;
            uint64_t accessed : 1;
            uint64_t dirty : 1;
            uint64_t user_mode_execute : 1;
            uint64_t ignored0 : 1;
            uint64_t reserved0 : 18;
            uint64_t address : 22; // [30:51] depends on maxphysaddr
            uint64_t ignored1 : 11;
            uint64_t suppress_ve : 1;
        } huge;
        uint64_t raw;
    };

    bool present() const;
    bool is_huge() const;

    physical_address_t address() const;
    void address(physical_address_t address);
};
static_assert(sizeof(pdpte_t) == 8, "sizeof(pdpte_t)");

struct pde_t {
    union {
        struct {
            // [SDM 3 28.2.2 P1164 "Table 28-5"]
            uint64_t read : 1;
            uint64_t write : 1;
            uint64_t execute : 1;
            uint64_t reserved0 : 5;
            uint64_t accessed : 1;
            uint64_t ignored0 : 1;
            uint64_t user_mode_execute : 1;
            uint64_t ignored1 : 1;
            uint64_t address : 40; // [12:51] depends on maxphysaddr
            uint64_t ignored2 : 12;
        } small;
        struct {
            // [SDM 3 28.2.2 P1163 "Table 28-4"]
            uint64_t read : 1;
            uint64_t write : 1;
            uint64_t execute : 1;
            uint64_t mem_type : 3;
            uint64_t ignore_pat : 1;
            uint64_t ps : 1;
            uint64_t accessed : 1;
            uint64_t dirty : 1;
            uint64_t user_mode_execute : 1;
            uint64_t ignored0 : 1;
            uint64_t reserved0 : 9;
            uint64_t address : 31; // [21:51] depends on maxphysaddr
            uint64_t ignored1 : 11;
            uint64_t suppress_ve : 1;
        } large;
        uint64_t raw;
    };

    bool present() const;
    bool is_large() const;

    physical_address_t address() const;
    void address(physical_address_t address);
};
static_assert(sizeof(pde_t) == 8, "sizeof(pde_t)");

struct pte_t {
    union {
        struct {
            // [SDM 3 28.2.2 P1165 "Table 28-6"]
            uint64_t read : 1;
            uint64_t write : 1;
            uint64_t execute : 1;
            uint64_t mem_type : 3;
            uint64_t ignore_pat : 1;
            uint64_t ignored0 : 1;
            uint64_t accessed : 1;
            uint64_t dirty : 1;
            uint64_t user_mode_execute : 1;
            uint64_t ignored1 : 1;
            uint64_t address : 40; // [12:51] depends on maxphysaddr
            uint64_t ignored2 : 11;
            uint64_t suppress_ve : 1;
        } bits;
        uint64_t raw;
    };

    bool present() const;

    physical_address_t address() const;
    void address(physical_address_t address);
};
static_assert(sizeof(pte_t) == 8, "sizeof(pte_t)");

// [SDM 3 24.6.11 P1067 "Table 24-8"]
struct ept_pointer_t {
    union {
        struct {
            uint64_t mem_type : 3;
            uint64_t walk_length : 3;
            uint64_t access_dirty_enable : 1;
            uint64_t reserved0 : 5;
            uint64_t address : 52; // [12:63] depends on maxphysaddr
        } bits;
        uint64_t raw;
    };

    physical_address_t address() const;
    void address(physical_address_t address);
};
static_assert(sizeof(ept_pointer_t) == 8, "sizeof(ept_pointer_t)");

enum class invept_type_t : uint64_t {
    single_context = 1,
    all_context = 2
};

struct invept_descriptor_t {
    ept_pointer_t eptp;
    uint64_t reserved;
};
static_assert(sizeof(invept_descriptor_t) == 16, "sizeof(invept_descriptor_t)");

enum class invvpid_type_t : uint64_t {
    individual_address = 0,
    single_context = 1,
    all_context = 2,
    single_context_retaining_globals = 3
};

struct invvpid_descriptor_t {
    uint16_t vpid;
    uint16_t reserved0;
    uint32_t reserved1;
    uint64_t address;
};
static_assert(sizeof(invvpid_descriptor_t) == 16, "sizeof(invvpid_descriptor_t)");

#pragma pack(pop)

bool to_physical(ept_pointer_t& eptp, guest_physical_address_t address, physical_address_t& out);

static inline instruction_result_t invept(invept_type_t type, invept_descriptor_t descriptor = {}) {
    auto error = instruction_result_t::success;
    asm volatile("invept %1, %2\n"
                 VMX_SET_ERROR_CODE
            : [error] "=r"(error) : "m" (descriptor), "r"(type) : "memory");
    return error;
}

static inline instruction_result_t invvpid(invept_type_t type, invvpid_descriptor_t descriptor = {}) {
    auto error = instruction_result_t::success;
    asm volatile("invvpid %1, %2\n"
                 VMX_SET_ERROR_CODE
            : [error] "=r"(error) : "m" (descriptor), "r"(type) : "memory");
    return error;
}

}
