#pragma once

#include "x86/common.h"
#include "x86/paging/paging.h"
#include "x86/cr.h"
#include "x86/vmx/error.h"

namespace x86::vmx {


// format for VMCS [SDM 3 24.2 P1054]
// format for vmxon region (size = vmcs size) [SDM 3 24.11.5 P1079]
struct vmstruct_t {
    uint32_t revision : 31;
    uint32_t shadow_indicator : 1;
    uint32_t abort_indicator;
    uint8_t data[x86::paging::page_size_4k - 8];
} packed;

bool is_supported() noexcept;

uintn_t get_cr0_fixed_bits(bool for_unrestricted_guest=false) noexcept;
uintn_t get_cr4_fixed_bits() noexcept;

void adjust_cr0_fixed_bits(x86::cr0_t& cr, bool for_unrestricted_guest=false) noexcept;
void adjust_cr4_fixed_bits(x86::cr4_t& cr) noexcept;

bool prepare_for_vmxon() noexcept;
bool initialize_vmstruct(vmstruct_t& vm_struct) noexcept;

static inline error_t vmxon(physical_address_t vmxon_region_address) {
    auto error = error_t::success;
    asm volatile("vmxon %1\n"
                 VMX_SET_ERROR_CODE
            : [error] "=r"(error) : "m" (*reinterpret_cast<uint64_t*>(&vmxon_region_address)) : "memory");
    return error;
}

static inline error_t vmxoff() {
    auto error = error_t::success;
    asm volatile("vmxoff");
    return error;
}

static inline error_t vmlaunch() {
    auto error = error_t::success;
    asm volatile("vmlaunch\n"
                 VMX_SET_ERROR_CODE
            : [error] "=r"(error) : : "memory");
    return error;
}

}
