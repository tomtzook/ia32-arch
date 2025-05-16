#pragma once

#include "x86/common.h"
#include "x86/paging/paging.h"
#include "x86/cr.h"
#include "x86/vmx/error.h"

namespace x86::vmx {

#pragma pack(push, 1)

// format for VMCS [SDM 3 24.2 P1054]
// format for vmxon region (size = vmcs size) [SDM 3 24.11.5 P1079]
struct vmstruct_t {
    uint32_t revision : 31;
    uint32_t shadow_indicator : 1;
    uint32_t abort_indicator;
    uint8_t data[x86::paging::page_size_4k - 8];
};

#pragma pack(pop)

bool is_supported();

uintn_t get_cr0_fixed0_bits(bool for_unrestricted_guest=false);
uintn_t get_cr0_fixed1_bits(bool for_unrestricted_guest=false);
uintn_t get_cr4_fixed0_bits();
uintn_t get_cr4_fixed1_bits();

void adjust_cr0_fixed_bits(x86::cr0_t& cr, bool for_unrestricted_guest=false);
void adjust_cr4_fixed_bits(x86::cr4_t& cr);

static inline bool is_cr_valid(const uintn_t cr, const uintn_t fixed0, const uintn_t fixed1) {
    if ((cr & fixed0) != fixed0) {
        return false;
    }
    if ((cr & ~fixed1) != 0) {
        return false;
    }

    return true;
}

static inline bool is_cr0_valid(const uintn_t cr, const bool for_unrestricted_guest=false) {
    const auto fixed0_bits = x86::vmx::get_cr0_fixed0_bits(for_unrestricted_guest);
    const auto fixed1_bits = x86::vmx::get_cr0_fixed1_bits(for_unrestricted_guest);
    return is_cr_valid(cr, fixed0_bits, fixed1_bits);
}

static inline bool is_cr4_valid(const uintn_t cr) {
    const auto fixed0_bits = x86::vmx::get_cr4_fixed0_bits();
    const auto fixed1_bits = x86::vmx::get_cr4_fixed1_bits();
    return is_cr_valid(cr, fixed0_bits, fixed1_bits);
}

bool prepare_for_vmxon(bool for_unrestricted_guest=false);
bool initialize_vmstruct(vmstruct_t& vm_struct);

static inline instruction_result_t vmxon(physical_address_t vmxon_region_address) {
    auto error = instruction_result_t::success;
    asm volatile("vmxon %1\n"
                 VMX_SET_ERROR_CODE
            : [error] "=r"(error) : "m" (*reinterpret_cast<uint64_t*>(&vmxon_region_address)) : "memory");
    return error;
}

static inline instruction_result_t vmxoff() {
    auto error = instruction_result_t::success;
    asm volatile("vmxoff");
    return error;
}

static inline instruction_result_t vmlaunch() {
    auto error = instruction_result_t::success;
    asm volatile("vmlaunch\n"
                 VMX_SET_ERROR_CODE
            : [error] "=r"(error) : : "memory");
    return error;
}

}
