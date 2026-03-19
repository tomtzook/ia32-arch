
#include "x86/cpuid.h"
#include "x86/id.h"

namespace x86 {

const char* cpu_series_str(const cpu_series series) {
    switch (series) {
        case cpu_series::unknown: return "unknown";
        case cpu_series::core_i_1: return "Intel Core I 1st Gen";
        case cpu_series::core_i_2: return "Intel Core I 2nd Gen";
        case cpu_series::core_i_3: return "Intel Core I 3rd Gen";
        case cpu_series::core_i_4: return "Intel Core I 4th Gen";
        case cpu_series::core_i_5: return "Intel Core I 5th Gen";
        case cpu_series::core_i_6: return "Intel Core I 6th Gen";
        case cpu_series::core_i_7: return "Intel Core I 7th Gen";
        case cpu_series::core_i_8: return "Intel Core I 8th Gen";
        case cpu_series::core_i_9: return "Intel Core I 9th Gen";
        case cpu_series::core_i_10: return "Intel Core I 10th Gen";
        case cpu_series::core_i_11: return "Intel Core I 11th Gen";
        case cpu_series::core_i_12: return "Intel Core I 12th Gen";
        case cpu_series::core_i_13: return "Intel Core I 13th Gen";
        case cpu_series::core_i_14: return "Intel Core I 14th Gen";
        case cpu_series::core_ultra_1: return "Intel Core Ultra Series 1";
        case cpu_series::core_ultra_2: return "Intel Core Ultra Series 2";
        case cpu_series::core_ultra_2_plus: return "Intel Core Ultra Series 2 Plus";
        case cpu_series::core_ultra_3: return "Intel Core Ultra Series 3";
        case cpu_series::core_ultra_4: return "Intel Core Ultra Series 4";
        default: return "";
    }
}

const char* cpu_microarchitecture_str(const cpu_microarchitecture microarchitecture) {
    switch (microarchitecture) {
        case cpu_microarchitecture::unknown: return "unknown";
        case cpu_microarchitecture::nehalem:  return "Nehalem";
        case cpu_microarchitecture::westmere: return "Westmere";
        case cpu_microarchitecture::sandy_bridge: return "Sandy Bridge";
        case cpu_microarchitecture::ivy_bridge: return "Ivy Bridge";
        case cpu_microarchitecture::haswell: return "Haswell";
        case cpu_microarchitecture::broadwell: return "Broadwell";
        case cpu_microarchitecture::skylake: return "Skylake";
        case cpu_microarchitecture::kaby_lake: return "Kaby Lake";
        case cpu_microarchitecture::kaby_lake_refresh: return "Kaby Lake-R";
        case cpu_microarchitecture::coffee_lake: return "Coffee Lake";
        case cpu_microarchitecture::whiskey_lake: return "Whiskey Lake";
        case cpu_microarchitecture::coffee_lake_refresh: return "Coffee Lake-R";
        case cpu_microarchitecture::commet_lake: return "Commet Lake";
        case cpu_microarchitecture::ice_lake: return "Ice Lake";
        case cpu_microarchitecture::rocket_lake: return "Rocket Lake";
        case cpu_microarchitecture::tiger_lake: return "Tiger Lake";
        case cpu_microarchitecture::alder_lake: return "Alder Lake";
        case cpu_microarchitecture::raptor_lake: return "Raptor Lake";
        case cpu_microarchitecture::raptor_lake_refresh: return "Raptor Lake-R";
        case cpu_microarchitecture::meteor_lake: return "Meteor Lake";
        case cpu_microarchitecture::lunar_lake: return "Lunar Lake";
        case cpu_microarchitecture::arrow_lake: return "Arrow Lake";
        case cpu_microarchitecture::arrow_lake_refresh: return "Arrow Lake-R";
        case cpu_microarchitecture::panther_lake: return "Panther Lake";
        case cpu_microarchitecture::nova_lake: return "Nova Lake";
        default: return "";
    }
}

cpu_series microarchitecture_to_series(const cpu_microarchitecture microarchitecture) {
    switch (microarchitecture) {
        case cpu_microarchitecture::nehalem:
        case cpu_microarchitecture::westmere:
            return cpu_series::core_i_1;
        case cpu_microarchitecture::sandy_bridge:
            return cpu_series::core_i_2;
        case cpu_microarchitecture::ivy_bridge:
            return cpu_series::core_i_3;
        case cpu_microarchitecture::haswell:
            return cpu_series::core_i_4;
        case cpu_microarchitecture::broadwell:
            return cpu_series::core_i_5;
        case cpu_microarchitecture::skylake:
            return cpu_series::core_i_6;
        case cpu_microarchitecture::kaby_lake:
        case cpu_microarchitecture::kaby_lake_refresh:
            return cpu_series::core_i_7;
        case cpu_microarchitecture::coffee_lake:
        case cpu_microarchitecture::whiskey_lake:
            return cpu_series::core_i_8;
        case cpu_microarchitecture::coffee_lake_refresh:
            return cpu_series::core_i_9;
        case cpu_microarchitecture::commet_lake:
        case cpu_microarchitecture::ice_lake:
            return cpu_series::core_i_10;
        case cpu_microarchitecture::rocket_lake:
        case cpu_microarchitecture::tiger_lake:
            return cpu_series::core_i_11;
        case cpu_microarchitecture::alder_lake:
            return cpu_series::core_i_12;
        case cpu_microarchitecture::raptor_lake:
            return cpu_series::core_i_13;
        case cpu_microarchitecture::raptor_lake_refresh:
            return cpu_series::core_i_14;
        case cpu_microarchitecture::meteor_lake:
            return cpu_series::core_ultra_1;
        case cpu_microarchitecture::lunar_lake:
        case cpu_microarchitecture::arrow_lake:
            return cpu_series::core_ultra_2;
        case cpu_microarchitecture::arrow_lake_refresh:
            return cpu_series::core_ultra_2_plus;
        case cpu_microarchitecture::panther_lake:
            return cpu_series::core_ultra_3;
        case cpu_microarchitecture::nova_lake:
            return cpu_series::core_ultra_4;
        default:
            return cpu_series::unknown;
    }
}

void store_cpu_vendor_str(char buffer[13]) {
    const auto regs = cpuid(0);

    *reinterpret_cast<uint32_t*>(buffer + 0) = regs.ebx;
    *reinterpret_cast<uint32_t*>(buffer + 4) = regs.edx;
    *reinterpret_cast<uint32_t*>(buffer + 8) = regs.ecx;
    buffer[12] = '\0';
}

bool is_intel_cpu() {
    char buffer[13];
    store_cpu_vendor_str(buffer);

    if (0 == __builtin_strcmp(buffer, vendor_string_intel)) {
        return true;
    }

    return false;
}

cpu_model get_cpu_model() {
    const auto cpu_features = cpuid<cpuid_eax01_t>();
    const auto type = static_cast<uint16_t>(cpu_features.eax.bits.processor_type);
    const auto family = static_cast<uint16_t>(cpu_features.eax.bits.family_id == 0x0f ?
        cpu_features.eax.bits.family_id + cpu_features.eax.bits.extended_family_id : cpu_features.eax.bits.family_id);
    const auto model = static_cast<uint16_t>((family == 6 || family == 15) ?
        (cpu_features.eax.bits.extended_model_id << 4) | cpu_features.eax.bits.model : cpu_features.eax.bits.model);
    const auto stepping = static_cast<uint16_t>(cpu_features.eax.bits.stepping_id);

    return {static_cast<cpu_processor_type>(type), static_cast<cpu_family>(family), model, stepping};
}

cpu_microarchitecture get_microarchitecture(const cpu_model model) {
    switch (model.family_id) {
        case cpu_family::modern_intel: {
            switch (model.model) {
                case 0x1a:
                case 0x1e:
                case 0x1f:
                case 0x2e:
                    return cpu_microarchitecture::nehalem;
                case 0x25:
                case 0x2c:
                case 0x2f:
                    return cpu_microarchitecture::westmere;
                case 0x2a:
                case 0x2d:
                    // stepping 0x6,0x7 at least
                    return cpu_microarchitecture::sandy_bridge;
                case 0x3a:
                case 0x3e:
                    // stepping 0x9 at least
                    return cpu_microarchitecture::ivy_bridge;
                case 0x3c:
                case 0x3f:
                case 0x45:
                case 0x46:
                    // stepping 0x1,0x2,0x3 at least
                    return cpu_microarchitecture::haswell;
                case 0x3d:
                case 0x47:
                case 0x4f:
                case 0x56:
                    // stepping 0x1,0x2 at least
                    return cpu_microarchitecture::broadwell;
                case 0x4e:
                case 0x5e:
                    // stepping 0x3 at least
                    return cpu_microarchitecture::skylake;
                case 0x8e: {
                    switch (model.stepping) {
                        case 0x9:
                            return cpu_microarchitecture::kaby_lake;
                        case 0xa:
                            return cpu_microarchitecture::kaby_lake_refresh;
                        case 0xb:
                        case 0xc:
                            return cpu_microarchitecture::whiskey_lake;
                        default:
                            return cpu_microarchitecture::unknown;
                    }
                }
                case 0x9e: {
                    switch (model.stepping) {
                        case 0x9:
                            return cpu_microarchitecture::kaby_lake;
                        case 0xa:
                        case 0xb:
                            return cpu_microarchitecture::coffee_lake;
                        case 0xc:
                        case 0xd:
                            return cpu_microarchitecture::coffee_lake_refresh;
                        default:
                            return cpu_microarchitecture::unknown;
                    }
                }
                case 0xa5:
                case 0xa6:
                    // stepping 0x0,0x1,0x2,0x3,0x4,0x5 at least
                    return cpu_microarchitecture::commet_lake;
                case 0x7d:
                case 0x7e:
                    // stepping 0x5 at least
                    return cpu_microarchitecture::ice_lake;
                case 0xa7:
                    // stepping 0x1 at least
                    return cpu_microarchitecture::rocket_lake;
                case 0x8c:
                case 0x8d:
                    // stepping 0x1 at least
                    return cpu_microarchitecture::tiger_lake;
                case 0x87:
                case 0x9a:
                    // stepping 0x0,0x1,0x2,0x3,0x4,0x5 at least
                    return cpu_microarchitecture::alder_lake;
                case 0xb7:
                case 0xbf:
                case 0xba: {
                    switch (model.stepping) {
                        case 0x1:
                        case 0x2:
                            return cpu_microarchitecture::raptor_lake;
                        default:
                            return cpu_microarchitecture::raptor_lake_refresh;
                    }
                }
                case 0xac:
                case 0xaa:
                    // stepping 0x4 at least
                    return cpu_microarchitecture::meteor_lake;
                case 0xbd:
                    // stepping 0x1 at least
                    return cpu_microarchitecture::lunar_lake;
                case 0xc5:
                case 0xc6: {
                    switch (model.stepping) {
                        case 0x0:
                        case 0x1:
                        case 0x2:
                            return cpu_microarchitecture::arrow_lake;
                        default:
                            return cpu_microarchitecture::arrow_lake_refresh;
                    }
                }
                case 0xd0:
                    return cpu_microarchitecture::panther_lake;
                default:
                    return cpu_microarchitecture::unknown;
            }
        }
        default:
            return cpu_microarchitecture::unknown;
    }
}

}
