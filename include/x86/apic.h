#pragma once

#include "types.h"


namespace x86::apic {

enum class mode_t {
    disabled,
    xapic,
    x2apic
};

mode_t current_mode() noexcept;

bool is_bsp() noexcept;

}
