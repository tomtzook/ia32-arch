
#include "x86/cr.h"
#include "x86/msr.h"
#include "x86/paging/paging.h"


namespace x86::paging {

mode_t get_current_mode() noexcept {
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

}
