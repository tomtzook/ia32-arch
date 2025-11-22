#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include "types.h"
#include "meta.h"


namespace x86 {

// only specific types will be able to specialize read/write
// use allow_struct_read_write(type) for new types.
// ReSharper disable once CppTemplateParameterNeverUsed
template<typename _t>
struct read_write_type_ : meta::false_type {};
#define allow_struct_read_write(x) \
    template<> \
    struct read_write_type_<x> : public meta::true_type {};

template<
        typename _t,
        typename meta::enable_if<
                read_write_type_<_t>::value,
                bool>::type = 0
>
_t read() = delete;

template<
        typename _t,
        typename meta::enable_if<
                read_write_type_<_t>::value,
                bool>::type = 0
>
void write(const _t& t) = delete;

}
