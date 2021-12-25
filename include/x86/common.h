#pragma once

#include "macros.h"
#include "types.h"
#include "intrinsics.h"
#include "meta.h"


namespace x86 {

template<typename _t>
_t read() noexcept;

template<typename _t>
void write(const _t& t) noexcept;

}
