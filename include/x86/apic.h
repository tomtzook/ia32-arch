#pragma once

#include "msr.h"


#define define_apic_register(_addr, _id, _name, _readable, _writable, _bits) \
    namespace x86::apic {                    \
        constexpr linear_address_t  _ ##_name## _xapic_addr_t = _addr; \
        constexpr x86::msr::id_t  _ ##_name## _x2apic_id_t = _id; \
        template<> struct register_def_t<_ ##_name## _xapic_addr_t, _ ##_name## _x2apic_id_t> : public x86::apic::_register_base_t<_ ##_name## _xapic_addr_t, _ ##_name## _x2apic_id_t> { \
            static constexpr auto readable = _readable; \
            static constexpr auto writable = _writable; \
            union {                     \
                _bits                   \
                x86::apic::value_t raw; \
            }; \
            explicit register_def_t(x86::apic::value_t val = 0) : raw(val) {}\
        }; \
        using _name## _t = x86::apic::register_def_t<_ ##_name## _xapic_addr_t, _ ##_name## _x2apic_id_t>; \
        static_assert(sizeof(_name## _t) == x86::apic::register_def_size, "sizeof(_name## _t)");\
    }

namespace x86 {
namespace apic {

enum class mode_t {
    disabled,
    xapic,
    x2apic
};

using value_t = uint32_t;
static constexpr size_t register_def_size = sizeof(value_t);

template<linear_address_t xapic_addr_, msr::id_t x2apic_id_>
struct _register_base_t {
    static constexpr auto xapic_addr = xapic_addr_;
    static constexpr auto x2apic_id = x2apic_id_;
};

template<linear_address_t xapic_addr_, msr::id_t x2apic_id_>
struct register_def_t : _register_base_t<xapic_addr_, x2apic_id_> {
    value_t raw;
};

template<typename t_>
struct is_register_def : meta::false_type {};
template<linear_address_t xapic_addr_, msr::id_t x2apic_id_>
struct is_register_def<register_def_t<xapic_addr_, x2apic_id_>> : meta::true_type {};

inline value_t xapic_read(const linear_address_t address) {
    return *reinterpret_cast<volatile value_t*>(address);
}

inline void xapic_write(const linear_address_t address, const value_t value) {
    *reinterpret_cast<volatile value_t*>(address) = value;
}

inline value_t x2apic_read(const msr::id_t id) {
    return msr::read(id);
}

inline void x2apic_write(const msr::id_t id, const value_t value) {
    msr::write(id, value);
}

template<
        typename _t,
        typename meta::enable_if<
                is_register_def<_t>::value,
                bool>::type = 0
>
_t xapic_read() {
    static_assert(sizeof(_t) == register_def_size, "bad APIC register size");
    static_assert(_t::readable, "register not readable");

    _t t;
    t.raw = apic::xapic_read(_t::xapic_addr);

    return t;
}

template<
        typename _t,
        typename meta::enable_if<
                is_register_def<_t>::value,
                bool>::type = 0
>
_t x2apic_read() {
    static_assert(sizeof(_t) == register_def_size, "bad APIC register size");
    static_assert(_t::readable, "register not readable");

    _t t;
    t.raw = apic::x2apic_read(_t::x2apic_id);

    return t;
}

template<
        typename _t,
        typename meta::enable_if<
                is_register_def<_t>::value,
                bool>::type = 0
>
void xapic_write(const _t& t) {
    static_assert(sizeof(_t) == register_def_size, "bad APIC register size");
    static_assert(_t::writable, "register not writable");

    apic::xapic_write(_t::xapic_addr, t.raw);
}

template<
        typename _t,
        typename meta::enable_if<
                is_register_def<_t>::value,
                bool>::type = 0
>
void x2apic_write(const _t& t) {
    static_assert(sizeof(_t) == register_def_size, "bad APIC register size");
    static_assert(_t::writable, "register not writable");

    apic::x2apic_write(_t::x2apic_id, t.raw);
}

bool is_x2apic_supported();

mode_t current_mode();
bool set_mode(mode_t mode);

uint32_t get_local_apic_id();
bool is_bsp();

}

template<
        typename _t,
        typename meta::enable_if<
                apic::is_register_def<_t>::value,
                bool>::type = 0
>
_t read() {
    static_assert(sizeof(_t) == apic::register_def_size, "bad APIC register size");
    switch (apic::current_mode()) {
        case apic::mode_t::xapic:
            return apic::xapic_read<_t>();
        case apic::mode_t::x2apic:
            return apic::x2apic_read<_t>();
        case apic::mode_t::disabled:
        default: {
            _t t;
            t.raw = 0;
            return t;
        }
    }
}

template<linear_address_t xapic_addr_, msr::id_t x2apic_id_>
apic::register_def_t<xapic_addr_, x2apic_id_> read() {
    return read<apic::register_def_t<xapic_addr_, x2apic_id_>>();
}

template<
        typename _t,
        typename meta::enable_if<
                apic::is_register_def<_t>::value,
                bool>::type = 0
>
void write(const _t& t) {
    static_assert(sizeof(_t) == apic::register_def_size, "bad APIC register size");

    switch (apic::current_mode()) {
        case apic::mode_t::disabled:
            break;
        case apic::mode_t::xapic:
            apic::xapic_write(t);
            break;
        case apic::mode_t::x2apic:
            apic::x2apic_write(t);
            break;
    }
}

template<linear_address_t xapic_addr_, msr::id_t x2apic_id_>
void write(apic::register_def_t<xapic_addr_, x2apic_id_>& t) {
    write(t);
}

}

define_apic_register(0xffe00080, 0x802, local_apic_id, true, true,
struct {
    value_t reserved0 : 24;
    value_t id : 8;
} xapic;
struct {
    value_t id : 32;
} x2apic;
)

define_apic_register(0xffe00080, 0x80f, spurious_interrupt_vector, true, true,
struct {
    value_t spurious_vector : 8;
    value_t apic_software_enable : 1;
    value_t focus_processor : 1;
    value_t reserved0 : 2;
    value_t eoi_broadcast_disable : 1;
    value_t reserved1 : 19;
} bits;
)
