#pragma once

#include "macros.h"
#include "types.h"
#include "intrinsics.h"


namespace x86 {

template<typename _t>
_t read();

template<typename _t>
void write(const _t& t);

}
