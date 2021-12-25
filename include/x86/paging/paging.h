#pragma once

#include "x86/common.h"

namespace x86::paging {

enum class mode_t {
    BIT32,
    PAE,
    IA32E
};

mode_t get_current_mode() noexcept;

}
