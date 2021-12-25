
#include "x86/cr.h"

namespace x86 {

cr0_t::cr0_t() noexcept
    : raw(0)
{}
cr0_t::cr0_t(uintn_t raw) noexcept
    : raw(raw)
{}

cr3_t::cr3_t() noexcept
    : raw(0)
{}
cr3_t::cr3_t(uintn_t raw) noexcept
    : raw(raw)
{}

cr4_t::cr4_t() noexcept
    : raw(0)
{}
cr4_t::cr4_t(uintn_t raw) noexcept
    : raw(raw)
{}

template<>
cr0_t read() {
    cr0_t reg;
    asm("mov %0, cr0"
        : "=r" (reg.raw));
    return reg;
}

template<>
void write(const cr0_t& t) {
    asm("mov cr0, %0"
        :
        : "r" (t.raw));
}

template<>
cr3_t read() {
    cr3_t reg;
    asm("mov %0, cr3"
        : "=r" (reg.raw));
    return reg;
}

template<>
void write(const cr3_t& t) {
    asm("mov cr3, %0"
        :
        : "r" (t.raw));
}

template<>
cr4_t read() {
    cr4_t reg;
    asm("mov %0, cr4"
        : "=r" (reg.raw));
    return reg;
}

template<>
void write(const cr4_t& t) {
    asm("mov cr4, %0"
        :
        : "r" (t.raw));
}

}
