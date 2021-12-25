#pragma once

#include "macros.h"
#include "types.h"
#include "intrinsics.h"
#include "meta.h"


namespace x86 {

template<typename _t>
struct __hack : public meta::false_type {};
#define allow_struct_read_write(x) \
    template<> \
    struct __hack<x> : public meta::true_type {};

template<
        typename _t,
        typename meta::enable_if<
                __hack<_t>::value,
                bool>::type = 0
>
_t read() noexcept = delete;

template<
        typename _t,
        typename meta::enable_if<
                __hack<_t>::value,
                bool>::type = 0
>
void write(const _t& t) noexcept = delete;

}
