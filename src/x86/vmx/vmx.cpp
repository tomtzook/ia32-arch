
#include "x86/cr.h"
#include "x86/cpuid.h"
#include "x86/msr.h"
#include "x86/vmx/vmx.h"


namespace x86::vmx {

bool is_supported() noexcept {
    // CPUID.1:ECX.VMX[bit 5] = 1 [SDM 3 23.6 P1050]
    auto cpu_features = x86::cpuid<x86::cpuid_eax01_t>();
    return cpu_features.ecx.bits.vmx;
}

uintn_t get_cr0_fixed_bits(bool for_unrestricted_guest) noexcept {
    // [SDM 3 A.7 P1960]
    // F0[X] = 1 -> must be 1
    // F1[X] = 0 -> must be 0
    auto fixed0 = x86::read<x86::msr::ia32_vmx_cr0_fixed0_t>().raw;
    auto fixed1 = x86::read<x86::msr::ia32_vmx_cr0_fixed1_t>().raw;

    if (for_unrestricted_guest) {
        // when in unrestricted guest mode, we don't need to account
        // for PG or PE bits
        cr0_t cr_0 = {fixed1};
        cr_0.bits.protection_enable = false;
        cr_0.bits.paging_enable = false;
        fixed1 = cr_0.raw;
    }

    return fixed0 & fixed1;
}

uintn_t get_cr4_fixed_bits() noexcept {
    // [SDM 3 A.8 P1960]
    auto fixed0 = x86::read<x86::msr::ia32_vmx_cr4_fixed0_t>().raw;
    auto fixed1 = x86::read<x86::msr::ia32_vmx_cr4_fixed1_t>().raw;

    return fixed0 & fixed1;
}

void adjust_cr0_fixed_bits(x86::cr0_t& cr, bool for_unrestricted_guest) noexcept {
    cr.raw |= get_cr0_fixed_bits(for_unrestricted_guest);
}

void adjust_cr4_fixed_bits(x86::cr4_t& cr) noexcept {
    cr.raw |= get_cr4_fixed_bits();
}

}
