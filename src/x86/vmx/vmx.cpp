
#include "x86/cr.h"
#include "x86/cpuid.h"
#include "x86/msr.h"
#include "x86/vmx/vmx.h"


namespace x86::vmx {

bool is_supported() {
    // CPUID.1:ECX.VMX[bit 5] = 1 [SDM 3 23.6 P1050]
    auto cpu_features = x86::cpuid<x86::cpuid_eax01_t>();
    return cpu_features.ecx.bits.vmx;
}

uintn_t get_cr0_fixed_bits(bool for_unrestricted_guest) {
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

uintn_t get_cr4_fixed_bits() {
    // [SDM 3 A.8 P1960]
    auto fixed0 = x86::read<x86::msr::ia32_vmx_cr4_fixed0_t>().raw;
    auto fixed1 = x86::read<x86::msr::ia32_vmx_cr4_fixed1_t>().raw;

    return fixed0 & fixed1;
}

void adjust_cr0_fixed_bits(x86::cr0_t& cr, bool for_unrestricted_guest) {
    cr.raw |= get_cr0_fixed_bits(for_unrestricted_guest);
}

void adjust_cr4_fixed_bits(x86::cr4_t& cr) {
    cr.raw |= get_cr4_fixed_bits();
}

bool prepare_for_vmxon() {
    // check IA32_FEATURE_CONTROL MSR [SDM 3 23.7 P1051]
    //      bit[0] = 0 -> vmxon #GP
    //      bit[1] = 0 -> vmxon [SMX mode] #GP
    //      bit[2] = 0 -> vmxon [non-SMX mode] #GP
    // SMX support CPUID.1:ECX[6] = 1 [SDM 2 6.2.1]
    //      SMX enabled CR4.SMXE[14] = 1

    auto feature_ctrl = x86::read<x86::msr::ia32_feature_ctrl_t>();
    auto cr4 = x86::read<x86::cr4_t>();

    if (!feature_ctrl.bits.lock_bit) {
        // lock bit is off, so we set what we need and lock
        if (cr4.bits.smx_enable) {
            feature_ctrl.bits.vmx_smx = true;
        } else {
            feature_ctrl.bits.vmx_no_smx = true;
        }
        feature_ctrl.bits.lock_bit = true;
        x86::write(feature_ctrl);
    } else if ((cr4.bits.smx_enable && !feature_ctrl.bits.vmx_smx) ||
               (!cr4.bits.smx_enable && !feature_ctrl.bits.vmx_no_smx)) {
        // ia32_feature_ctrl does not support the current SMX mode
        return false;
    }

    // Restrictions placed on CR0 and CR4 [SDM 3 23.8 P1051]
    auto cr0 = x86::read<cr0_t>();
    x86::vmx::adjust_cr0_fixed_bits(cr0);
    x86::write(cr0);

    x86::vmx::adjust_cr4_fixed_bits(cr4);
    // enable vmx CR4.VMXE[13] = 1 [SDM 3 23.7 P1051]
    cr4.bits.vmx_enable = 1;
    x86::write(cr4);

    return true;
}

bool initialize_vmstruct(vmstruct_t& vm_struct) {
    auto vmx_basic = x86::read<x86::msr::ia32_vmx_basic_t>();
    if (sizeof(vm_struct) > vmx_basic.bits.vm_struct_size) {
        return false;
    }

    vm_struct.revision = vmx_basic.bits.vmcs_revision;
    vm_struct.shadow_indicator = false;

    return true;
}

}
