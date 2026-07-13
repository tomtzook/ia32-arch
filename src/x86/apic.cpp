
#include "x86/msr.h"
#include "x86/cpuid.h"
#include "x86/apic.h"


namespace x86::apic {

bool is_x2apic_supported() {
    const auto cpu_features = x86::cpuid<cpuid_eax01_t>();
    return cpu_features.ecx.bits.x2apic;
}

mode_t current_mode() {
    // todo: cache mode!
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

bool set_mode(const mode_t mode) {
    switch (mode) {
        case mode_t::disabled: {
            auto apic_base = read<msr::ia32_apic_base_t>();
            apic_base.bits.global_enable = false;
            apic_base.bits.extd = false;
            write(apic_base);

            return true;
        }
        case mode_t::xapic: {
            auto apic_base = read<msr::ia32_apic_base_t>();
            if (apic_base.bits.global_enable && apic_base.bits.extd) {
                // we are in x2apic, we need turn of and then enter it
                apic_base.bits.global_enable = false;
                apic_base.bits.extd = false;
                write(apic_base);

                apic_base.bits.global_enable = true;
                apic_base.bits.extd = false;
                write(apic_base);
            } else {
                apic_base.bits.global_enable = true;
                apic_base.bits.extd = false;
                write(apic_base);
            }

            return true;
        }
        case mode_t::x2apic: {
            if (!is_x2apic_supported()) {
                return false;
            }

            auto apic_base = read<msr::ia32_apic_base_t>();
            apic_base.bits.global_enable = true;
            apic_base.bits.extd = true;
            write(apic_base);

            return true;
        }
        default:
            return false;
    }
}

uint32_t get_local_apic_id() {
    switch (current_mode()) {
        case mode_t::xapic:
            return xapic_read<local_apic_id_t>().xapic.id;
        case mode_t::x2apic:
            return x2apic_read<local_apic_id_t>().x2apic.id;
        case mode_t::disabled:
        default:
            return 0;
    }
}

bool is_bsp() {
    const auto apic_base = read<msr::ia32_apic_base_t>();
    return apic_base.bits.bsp;
}

void send_ipi(const uint8_t vector, const delivery_mode_t delivery_mode, const destination_mode_t destination_mode, const level_t level, const trigger_mode_t trigger_mode, const destination_shorthand_t destination) {
    icr_low_t icr{};
    icr.bits.vector = vector;
    icr.bits.delivery_mode = delivery_mode;
    icr.bits.destination_mode = destination_mode;
    icr.bits.level = level;
    icr.bits.trigger_mode = trigger_mode;
    icr.bits.destination_shorthand = destination;

    switch (current_mode()) {
        case mode_t::xapic: {
            icr_low_t low_read{};
            do {
                __asm__ __volatile__("pause");
                low_read = xapic_read<icr_low_t>();
            } while (low_read.bits.delivery_status != delivery_status_t::idle);

            xapic_write(icr);
            break;
        }
        case mode_t::x2apic: {
            x2apic_write(icr);
            break;
        }
        case mode_t::disabled:
        default:
            break;
    }
}

}
