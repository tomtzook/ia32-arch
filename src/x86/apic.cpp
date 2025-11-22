
#include "x86/msr.h"
#include "x86/cpuid.h"
#include "x86/apic.h"


namespace x86::apic {

mode_t current_mode() {
    // [SDM 3 10.12.1 P398]
    const auto apic_base = read<msr::ia32_apic_base_t>();
    if (apic_base.bits.global_enable) {
        const auto cpu_features = x86::cpuid<cpuid_eax01_t>();
        if (apic_base.bits.extd && cpu_features.ecx.bits.x2apic) {
            return mode_t::x2apic;
        }

        return mode_t::xapic;
    }

    return mode_t::disabled;
}

bool is_bsp() {
    const auto apic_base = read<msr::ia32_apic_base_t>();
    return apic_base.bits.bsp;
}

}
