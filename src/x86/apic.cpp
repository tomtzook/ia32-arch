
#include "x86/msr.h"
#include "x86/apic.h"


namespace x86::apic {

bool is_bsp() noexcept {
    auto apic_base = read<msr::ia32_apic_base_t>();
    return apic_base.bits.bsp;
}

}
