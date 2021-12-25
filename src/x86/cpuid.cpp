
#include "x86/cpuid.h"


namespace x86 {

cpuid_regs_t cpuid(cpuid_t leaf, cpuid_t sub_leaf) noexcept {
    cpuid_regs_t regs{};
    __asm__("cpuid;"
            : "=a" (regs.eax), "=b" (regs.ebx), "=c" (regs.ecx), "=d" (regs.edx)
            : "0" (leaf), "2" (sub_leaf));

    return regs;
}

}
