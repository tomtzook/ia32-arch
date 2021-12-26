
#include "x86/cr.h"
#include "x86/msr.h"
#include "x86/cpuid.h"
#include "x86/paging/paging.h"


namespace x86::paging {

mode_t current_mode() noexcept {
    // PAE paging mode
    //  CR0.PG = 1, CR4.PAE = 1, IA32_EFER.LME = 0
    // IA32e paging mode [SDM 3 4.1.1 P106]
    //  CR0.PG = 1, CR4.PAE = 1, IA32_EFER.LME = 1
    auto cr0 = read<cr0_t>();
    auto cr4 = read<cr4_t>();
    auto efer = read<msr::ia32_efer_t>();

    if (cr0.bits.paging_enable && cr4.bits.physical_address_extension) {
        if (efer.bits.lma) {
            return mode_t::IA32E;
        } else {
            return mode_t::PAE;
        }
    } else if (cr0.bits.paging_enable) {
        return mode_t::BIT32;
    } else {
        return mode_t::DISABLED;
    }
}

size_t max_physical_address_width() noexcept {
    // [SDM 3 4.1.4 P109]
    // MAXPHYADDR

    // is leaf supported?
    // if not return 36 if cpuid_eax01_t.edx.pae = 1, or 32 otherwise
    if (x86::is_cpuid_leaf_supported(0x80000008)) {
        auto cpuid_maxphysaddr = x86::cpuid<0x80000008>();
        return cpuid_maxphysaddr.eax & 0xff;
    }

    auto cpuid_features = x86::cpuid<x86::cpuid_eax01_t>();
    return cpuid_features.edx.bits.pae ? 36 : 32;
}

}
