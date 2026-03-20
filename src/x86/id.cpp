
#include "x86/cpuid.h"
#include "x86/id.h"

namespace x86 {

const char* cpu_series_str(const cpu_series_t series) {
    switch (series) {
        case cpu_series_t::unknown: return "unknown";
        case cpu_series_t::core_i_1: return "Intel Core I 1st Gen";
        case cpu_series_t::core_i_2: return "Intel Core I 2nd Gen";
        case cpu_series_t::core_i_3: return "Intel Core I 3rd Gen";
        case cpu_series_t::core_i_4: return "Intel Core I 4th Gen";
        case cpu_series_t::core_i_5: return "Intel Core I 5th Gen";
        case cpu_series_t::core_i_6: return "Intel Core I 6th Gen";
        case cpu_series_t::core_i_7: return "Intel Core I 7th Gen";
        case cpu_series_t::core_i_8: return "Intel Core I 8th Gen";
        case cpu_series_t::core_i_9: return "Intel Core I 9th Gen";
        case cpu_series_t::core_i_10: return "Intel Core I 10th Gen";
        case cpu_series_t::core_i_11: return "Intel Core I 11th Gen";
        case cpu_series_t::core_i_12: return "Intel Core I 12th Gen";
        case cpu_series_t::core_i_13: return "Intel Core I 13th Gen";
        case cpu_series_t::core_i_14: return "Intel Core I 14th Gen";
        case cpu_series_t::core_ultra_1: return "Intel Core Ultra Series 1";
        case cpu_series_t::core_ultra_2: return "Intel Core Ultra Series 2";
        case cpu_series_t::core_ultra_2_plus: return "Intel Core Ultra Series 2 Plus";
        case cpu_series_t::core_ultra_3: return "Intel Core Ultra Series 3";
        case cpu_series_t::core_ultra_4: return "Intel Core Ultra Series 4";
        default: return "";
    }
}

const char* cpu_microarchitecture_str(const cpu_microarchitecture_t microarchitecture) {
    switch (microarchitecture) {
        case cpu_microarchitecture_t::unknown: return "unknown";
        case cpu_microarchitecture_t::nehalem:  return "Nehalem";
        case cpu_microarchitecture_t::westmere: return "Westmere";
        case cpu_microarchitecture_t::sandy_bridge: return "Sandy Bridge";
        case cpu_microarchitecture_t::ivy_bridge: return "Ivy Bridge";
        case cpu_microarchitecture_t::haswell: return "Haswell";
        case cpu_microarchitecture_t::broadwell: return "Broadwell";
        case cpu_microarchitecture_t::skylake: return "Skylake";
        case cpu_microarchitecture_t::kaby_lake: return "Kaby Lake";
        case cpu_microarchitecture_t::kaby_lake_refresh: return "Kaby Lake-R";
        case cpu_microarchitecture_t::coffee_lake: return "Coffee Lake";
        case cpu_microarchitecture_t::whiskey_lake: return "Whiskey Lake";
        case cpu_microarchitecture_t::coffee_lake_refresh: return "Coffee Lake-R";
        case cpu_microarchitecture_t::commet_lake: return "Commet Lake";
        case cpu_microarchitecture_t::ice_lake: return "Ice Lake";
        case cpu_microarchitecture_t::rocket_lake: return "Rocket Lake";
        case cpu_microarchitecture_t::tiger_lake: return "Tiger Lake";
        case cpu_microarchitecture_t::alder_lake: return "Alder Lake";
        case cpu_microarchitecture_t::raptor_lake: return "Raptor Lake";
        case cpu_microarchitecture_t::raptor_lake_refresh: return "Raptor Lake-R";
        case cpu_microarchitecture_t::meteor_lake: return "Meteor Lake";
        case cpu_microarchitecture_t::lunar_lake: return "Lunar Lake";
        case cpu_microarchitecture_t::arrow_lake: return "Arrow Lake";
        case cpu_microarchitecture_t::arrow_lake_refresh: return "Arrow Lake-R";
        case cpu_microarchitecture_t::panther_lake: return "Panther Lake";
        case cpu_microarchitecture_t::nova_lake: return "Nova Lake";
        default: return "";
    }
}

cpu_series_t microarchitecture_to_series(const cpu_microarchitecture_t microarchitecture) {
    switch (microarchitecture) {
        case cpu_microarchitecture_t::nehalem:
        case cpu_microarchitecture_t::westmere:
            return cpu_series_t::core_i_1;
        case cpu_microarchitecture_t::sandy_bridge:
            return cpu_series_t::core_i_2;
        case cpu_microarchitecture_t::ivy_bridge:
            return cpu_series_t::core_i_3;
        case cpu_microarchitecture_t::haswell:
            return cpu_series_t::core_i_4;
        case cpu_microarchitecture_t::broadwell:
            return cpu_series_t::core_i_5;
        case cpu_microarchitecture_t::skylake:
            return cpu_series_t::core_i_6;
        case cpu_microarchitecture_t::kaby_lake:
        case cpu_microarchitecture_t::kaby_lake_refresh:
            return cpu_series_t::core_i_7;
        case cpu_microarchitecture_t::coffee_lake:
        case cpu_microarchitecture_t::whiskey_lake:
            return cpu_series_t::core_i_8;
        case cpu_microarchitecture_t::coffee_lake_refresh:
            return cpu_series_t::core_i_9;
        case cpu_microarchitecture_t::commet_lake:
        case cpu_microarchitecture_t::ice_lake:
            return cpu_series_t::core_i_10;
        case cpu_microarchitecture_t::rocket_lake:
        case cpu_microarchitecture_t::tiger_lake:
            return cpu_series_t::core_i_11;
        case cpu_microarchitecture_t::alder_lake:
            return cpu_series_t::core_i_12;
        case cpu_microarchitecture_t::raptor_lake:
            return cpu_series_t::core_i_13;
        case cpu_microarchitecture_t::raptor_lake_refresh:
            return cpu_series_t::core_i_14;
        case cpu_microarchitecture_t::meteor_lake:
            return cpu_series_t::core_ultra_1;
        case cpu_microarchitecture_t::lunar_lake:
        case cpu_microarchitecture_t::arrow_lake:
            return cpu_series_t::core_ultra_2;
        case cpu_microarchitecture_t::arrow_lake_refresh:
            return cpu_series_t::core_ultra_2_plus;
        case cpu_microarchitecture_t::panther_lake:
            return cpu_series_t::core_ultra_3;
        case cpu_microarchitecture_t::nova_lake:
            return cpu_series_t::core_ultra_4;
        default:
            return cpu_series_t::unknown;
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

cpu_model_t get_cpu_model() {
    const auto cpu_features = cpuid<cpuid_eax01_t>();
    const auto type = static_cast<uint16_t>(cpu_features.eax.bits.processor_type);
    const auto family = static_cast<uint16_t>(cpu_features.eax.bits.family_id == 0x0f ?
        cpu_features.eax.bits.family_id + cpu_features.eax.bits.extended_family_id : cpu_features.eax.bits.family_id);
    const auto model = static_cast<uint16_t>((family == 6 || family == 15) ?
        (cpu_features.eax.bits.extended_model_id << 4) | cpu_features.eax.bits.model : cpu_features.eax.bits.model);
    const auto stepping = static_cast<uint16_t>(cpu_features.eax.bits.stepping_id);

    return {static_cast<cpu_processor_type_t>(type), static_cast<cpu_family_t>(family), model, stepping};
}

cpu_microarchitecture_t get_microarchitecture(const cpu_model_t model) {
    switch (model.family_id) {
        case cpu_family_t::modern_intel: {
            switch (model.model) {
                case 0x1a:
                case 0x1e:
                case 0x1f:
                case 0x2e:
                    return cpu_microarchitecture_t::nehalem;
                case 0x25:
                case 0x2c:
                case 0x2f:
                    return cpu_microarchitecture_t::westmere;
                case 0x2a:
                case 0x2d:
                    // stepping 0x6,0x7 at least
                    return cpu_microarchitecture_t::sandy_bridge;
                case 0x3a:
                case 0x3e:
                    // stepping 0x9 at least
                    return cpu_microarchitecture_t::ivy_bridge;
                case 0x3c:
                case 0x3f:
                case 0x45:
                case 0x46:
                    // stepping 0x1,0x2,0x3 at least
                    return cpu_microarchitecture_t::haswell;
                case 0x3d:
                case 0x47:
                case 0x4f:
                case 0x56:
                    // stepping 0x1,0x2 at least
                    return cpu_microarchitecture_t::broadwell;
                case 0x4e:
                case 0x5e:
                    // stepping 0x3 at least
                    return cpu_microarchitecture_t::skylake;
                case 0x8e: {
                    switch (model.stepping) {
                        case 0x9:
                            return cpu_microarchitecture_t::kaby_lake;
                        case 0xa:
                            return cpu_microarchitecture_t::kaby_lake_refresh;
                        case 0xb:
                        case 0xc:
                            return cpu_microarchitecture_t::whiskey_lake;
                        default:
                            return cpu_microarchitecture_t::unknown;
                    }
                }
                case 0x9e: {
                    switch (model.stepping) {
                        case 0x9:
                            return cpu_microarchitecture_t::kaby_lake;
                        case 0xa:
                        case 0xb:
                            return cpu_microarchitecture_t::coffee_lake;
                        case 0xc:
                        case 0xd:
                            return cpu_microarchitecture_t::coffee_lake_refresh;
                        default:
                            return cpu_microarchitecture_t::unknown;
                    }
                }
                case 0xa5:
                case 0xa6:
                    // stepping 0x0,0x1,0x2,0x3,0x4,0x5 at least
                    return cpu_microarchitecture_t::commet_lake;
                case 0x7d:
                case 0x7e:
                    // stepping 0x5 at least
                    return cpu_microarchitecture_t::ice_lake;
                case 0xa7:
                    // stepping 0x1 at least
                    return cpu_microarchitecture_t::rocket_lake;
                case 0x8c:
                case 0x8d:
                    // stepping 0x1 at least
                    return cpu_microarchitecture_t::tiger_lake;
                case 0x87:
                case 0x9a:
                    // stepping 0x0,0x1,0x2,0x3,0x4,0x5 at least
                    return cpu_microarchitecture_t::alder_lake;
                case 0xb7:
                case 0xbf:
                case 0xba: {
                    switch (model.stepping) {
                        case 0x1:
                        case 0x2:
                            return cpu_microarchitecture_t::raptor_lake;
                        default:
                            return cpu_microarchitecture_t::raptor_lake_refresh;
                    }
                }
                case 0xac:
                case 0xaa:
                    // stepping 0x4 at least
                    return cpu_microarchitecture_t::meteor_lake;
                case 0xbd:
                    // stepping 0x1 at least
                    return cpu_microarchitecture_t::lunar_lake;
                case 0xc5:
                case 0xc6: {
                    switch (model.stepping) {
                        case 0x0:
                        case 0x1:
                        case 0x2:
                            return cpu_microarchitecture_t::arrow_lake;
                        default:
                            return cpu_microarchitecture_t::arrow_lake_refresh;
                    }
                }
                case 0xd0:
                    return cpu_microarchitecture_t::panther_lake;
                default:
                    return cpu_microarchitecture_t::unknown;
            }
        }
        default:
            return cpu_microarchitecture_t::unknown;
    }
}

}
