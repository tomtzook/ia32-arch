#pragma once

namespace meta {

template<typename t_, t_ v_>
struct integral_constant {
    static constexpr t_ value = v_;
    using value_type = t_;
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

// ReSharper disable once CppTemplateParameterNeverUsed
template<bool, typename t_ = void>
struct enable_if {
};

template<typename t_>
struct enable_if<true, t_> {
    using type = t_;
};

}
