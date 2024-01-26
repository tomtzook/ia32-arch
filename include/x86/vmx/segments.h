#pragma once

#include "x86/common.h"
#include "x86/segments.h"
#include "x86/vmx/vmcs.h"

namespace x86::vmx {

#pragma pack(push, 1)

// [SDM 3 24.4.1]
struct segment_access_rights_t {
    union {
        struct {
            uint32_t type : 4; //x86::segments::type_t
            uint32_t s : 1; //x86::segments::descriptor_type_t
            uint32_t dpl : 2;
            uint32_t present : 1;
            uint32_t reserved0 : 4;
            uint32_t available : 1;
            uint32_t long_mode : 1; // cs only
            uint32_t default_db : 1;
            uint32_t granularity : 1; //x86::segments::granularity_t
            uint32_t unusable : 1;
            uint32_t reserved1 : 15;
        } bits;
        uint32_t raw;
    };
};
static_assert(sizeof(segment_access_rights_t) == 4, "sizeof(segment_access_rights_t)");

template<
        typename _segment,
        field_t _guest_selector,
        field_t _guest_base,
        field_t _guest_limit,
        field_t _guest_ar,
        field_t _host_selector>
struct vmcs_segment_defs {
    using segment = _segment;
    static constexpr field_t guest_selector = _guest_selector;
    static constexpr field_t guest_base = _guest_base;
    static constexpr field_t guest_limit = _guest_limit;
    static constexpr field_t guest_ar = _guest_ar;
    static constexpr field_t host_selector = _host_selector;
};

using vmcs_cs_segment = vmcs_segment_defs<
        x86::segments::cs_t,
        field_t::guest_cs_selector,
        field_t::guest_cs_base,
        field_t::guest_cs_limit,
        field_t::guest_cs_access_rights,
        field_t::host_cs_selector>;

using vmcs_ds_segment = vmcs_segment_defs<
        x86::segments::ds_t,
        field_t::guest_ds_selector,
        field_t::guest_ds_base,
        field_t::guest_ds_limit,
        field_t::guest_ds_access_rights,
        field_t::host_ds_selector>;

using vmcs_gs_segment = vmcs_segment_defs<
        x86::segments::gs_t,
        field_t::guest_gs_selector,
        field_t::guest_gs_base,
        field_t::guest_gs_limit,
        field_t::guest_gs_access_rights,
        field_t::host_gs_selector>;

using vmcs_ss_segment = vmcs_segment_defs<
        x86::segments::ss_t,
        field_t::guest_ss_selector,
        field_t::guest_ss_base,
        field_t::guest_ss_limit,
        field_t::guest_ss_access_rights,
        field_t::host_ss_selector>;

using vmcs_es_segment = vmcs_segment_defs<
        x86::segments::es_t,
        field_t::guest_es_selector,
        field_t::guest_es_base,
        field_t::guest_es_limit,
        field_t::guest_es_access_rights,
        field_t::host_es_selector>;

using vmcs_fs_segment = vmcs_segment_defs<
        x86::segments::fs_t,
        field_t::guest_fs_selector,
        field_t::guest_fs_base,
        field_t::guest_fs_limit,
        field_t::guest_fs_access_rights,
        field_t::host_fs_selector>;

#pragma pack(pop)

static inline x86::vmx::segment_access_rights_t segment_access_rights(const segments::descriptor_t& descriptor) noexcept {
    segment_access_rights_t ar{};
    ar.bits.type = static_cast<uint32_t>(descriptor.bits.type);
    ar.bits.s = static_cast<uint32_t>(descriptor.bits.s);
    ar.bits.dpl = descriptor.bits.dpl;
    ar.bits.present = descriptor.bits.present;
    ar.bits.available = descriptor.bits.available;
    ar.bits.long_mode = descriptor.bits.long_mode;
    ar.bits.default_db = descriptor.bits.default_db;
    ar.bits.granularity = static_cast<uint32_t>(descriptor.bits.granularity);
    ar.bits.unusable = false;

    return ar;
}

}
