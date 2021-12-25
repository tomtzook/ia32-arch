#pragma once

#include "types.h"

namespace meta {

template<typename _t, _t _v>
struct integral_constant {
    static constexpr _t value = _v;
    using value_type = _t;
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template<bool, typename _t = void>
struct enable_if {
};

template<typename _t>
struct enable_if<true, _t> {
    using type = _t;
};

}
