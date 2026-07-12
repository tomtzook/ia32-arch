
#include "x86/dr.h"

namespace x86 {

bool dr7_t::is_bp_active(const unsigned idx) const {
   switch (idx) {
       case 0:
           return bits.local_enable_bp_0 || bits.global_enable_bp_0;
        case 1:
           return bits.local_enable_bp_1 || bits.global_enable_bp_1;
       case 2:
           return bits.local_enable_bp_2 || bits.global_enable_bp_2;
       case 3:
           return bits.local_enable_bp_3 || bits.global_enable_bp_3;
        default:
           return false;
   }
}

void dr7_t::set_bp(const unsigned idx, const bp_condition_t condition, const bp_length_t len) {
    switch (idx) {
        case 0:
            bits.condition_bp_0 = condition;
            bits.length_bp_0 = len;
            bits.local_enable_bp_0 = true;
            break;
        case 1:
            bits.condition_bp_1 = condition;
            bits.length_bp_1 = len;
            bits.local_enable_bp_1 = true;
            break;
        case 2:
            bits.condition_bp_2 = condition;
            bits.length_bp_2 = len;
            bits.local_enable_bp_2 = true;
            break;
        case 3:
            bits.condition_bp_3 = condition;
            bits.length_bp_3 = len;
            bits.local_enable_bp_3 = true;
            break;
        default:
            break;
    }
}

void dr7_t::clear_bp(const unsigned idx) {
    switch (idx) {
        case 0:
            bits.local_enable_bp_0 = false;
            bits.global_enable_bp_0 = false;
            break;
        case 1:
            bits.local_enable_bp_1 = false;
            bits.global_enable_bp_1 = false;
            break;
        case 2:
            bits.local_enable_bp_2 = false;
            bits.global_enable_bp_2 = false;
            break;
        case 3:
            bits.local_enable_bp_3 = false;
            bits.global_enable_bp_3 = false;
            break;
        default:
            break;
    }
}

}
