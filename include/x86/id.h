#pragma once

#include "types.h"

namespace x86 {

static const char* vendor_string_intel = "GenuineIntel";
static const char* vendor_string_amd = "AuthenticAMD";

enum class cpu_processor_type_t {
    original_oem = 0,
    intel_overdrive = 1,
    dual_processor = 2,
};

enum class cpu_family_t {
    intel486 = 0x4,
    pentium = 0x5,
    modern_intel = 0x6,
    itanium = 0x7,
    pentium4 = 0xf,
    itanium2 = 0x1f,
};

enum class cpu_series_t {
    unknown = 0,
    core_i_1 = 1, // Intel Core I 1st Generation
    core_i_2, // Intel Core I 2nd Generation
    core_i_3, // Intel Core I 3rd Generation
    core_i_4, // Intel Core I 4th Generation
    core_i_5, // Intel Core I 5th Generation
    core_i_6, // Intel Core I 6th Generation
    core_i_7, // Intel Core I 7th Generation
    core_i_8, // Intel Core I 8th Generation
    core_i_9, // Intel Core I 9th Generation
    core_i_10, // Intel Core I 10th Generation
    core_i_11, // Intel Core I 11th Generation
    core_i_12, // Intel Core I 12th Generation
    core_i_13, // Intel Core I 13th Generation
    core_i_14, // Intel Core I 14th Generation
    core_ultra_1, // Intel Core Ultra Series 1
    core_ultra_2, // Intel Core Ultra Series 2
    core_ultra_2_plus, // Intel Core Ultra Series 2+
    core_ultra_3, // Intel Core Ultra Series 3
    core_ultra_4, // Intel Core Ultra Series 4
};

enum class cpu_microarchitecture_t {
    unknown = 0,
    nehalem = 1,
    westmere,
    sandy_bridge,
    ivy_bridge,
    haswell,
    broadwell,
    skylake,
    kaby_lake,
    kaby_lake_refresh,
    coffee_lake,
    whiskey_lake,
    coffee_lake_refresh,
    commet_lake,
    ice_lake,
    rocket_lake,
    tiger_lake,
    alder_lake,
    raptor_lake,
    raptor_lake_refresh,
    meteor_lake,
    lunar_lake,
    arrow_lake,
    arrow_lake_refresh,
    panther_lake,
    nova_lake
};

struct cpu_model_t {
    cpu_processor_type_t type;
    cpu_family_t family_id;
    uint16_t model;
    uint16_t stepping;
};

const char* cpu_series_str(cpu_series_t series);
const char* cpu_microarchitecture_str(cpu_microarchitecture_t microarchitecture);
cpu_series_t microarchitecture_to_series(cpu_microarchitecture_t microarchitecture);

void store_cpu_vendor_str(char buffer[13]);
bool is_intel_cpu();

cpu_model_t get_cpu_model();
cpu_microarchitecture_t get_microarchitecture(cpu_model_t model);

}
