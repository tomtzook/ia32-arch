
#include "x86/cpuid.h"


namespace x86 {

cpuid_t max_supported_cpuid_leaf() {
    auto cpuid = x86::cpuid<0x80000000>();
    return cpuid.eax;
}

bool is_cpuid_leaf_supported(cpuid_t leaf) {
    return leaf >= max_supported_cpuid_leaf();
}

}
