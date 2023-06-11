#pragma once

#include "x86/common.h"
#include "x86/cr.h"


namespace x86::vmx {

bool is_supported() noexcept;

uintn_t get_cr0_fixed_bits(bool for_unrestricted_guest=false) noexcept;
uintn_t get_cr4_fixed_bits() noexcept;

void adjust_cr0_fixed_bits(x86::cr0_t& cr, bool for_unrestricted_guest=false) noexcept;
void adjust_cr4_fixed_bits(x86::cr4_t& cr) noexcept;

}
