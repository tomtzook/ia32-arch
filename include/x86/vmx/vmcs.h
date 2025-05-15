#pragma once

#include "x86/common.h"
#include "x86/vmx/error.h"


namespace x86::vmx {

// https://gist.github.com/wbenny/d0fdb53425de2641e207b8c4ef672cbc
enum class field_t : uint32_t {
    ctrl_virtual_processor_identifier = 0x0,
    ctrl_posted_interrupt_notification_vector = 0x2,
    ctrl_eptp_index = 0x4,
    guest_es_selector = 0x800,
    guest_cs_selector = 0x802,
    guest_ss_selector = 0x804,
    guest_ds_selector = 0x806,
    guest_fs_selector = 0x808,
    guest_gs_selector = 0x80a,
    guest_ldtr_selector = 0x80c,
    guest_tr_selector = 0x80e,
    guest_interrupt_status = 0x810,
    guest_pml_index = 0x812,
    host_es_selector = 0xc00,
    host_cs_selector = 0xc02,
    host_ss_selector = 0xc04,
    host_ds_selector = 0xc06,
    host_fs_selector = 0xc08,
    host_gs_selector = 0xc0a,
    host_tr_selector = 0xc0c,
    ctrl_io_bitmap_a_address = 0x2000,
    ctrl_io_bitmap_b_address = 0x2002,
    ctrl_msr_bitmap_address = 0x2004,
    ctrl_vmexit_msr_store_address = 0x2006,
    ctrl_vmexit_msr_load_address = 0x2008,
    ctrl_vmentry_msr_load_address = 0x200a,
    ctrl_executive_pointer = 0x200c,
    ctrl_pml_address = 0x200e,
    ctrl_tsc_offset = 0x2010,
    ctrl_virtual_apic_address = 0x2012,
    ctrl_apic_access_address = 0x2014,
    ctrl_posted_interrupt_descriptor_address = 0x2016,
    ctrl_vmfunc_controls = 0x2018,
    ctrl_ept_pointer = 0x201a,
    ctrl_eoi_exit_bitmap_0 = 0x201c,
    ctrl_eoi_exit_bitmap_1 = 0x201e,
    ctrl_eoi_exit_bitmap_2 = 0x2020,
    ctrl_eoi_exit_bitmap_3 = 0x2022,
    ctrl_ept_pointer_list_address = 0x2024,
    ctrl_vmread_bitmap_address = 0x2026,
    ctrl_vmwrite_bitmap_address = 0x2028,
    ctrl_virtualization_exception_information_address = 0x202a,
    ctrl_xss_exiting_bitmap = 0x202c,
    ctrl_encls_exiting_bitmap = 0x202e,
    ctrl_tsc_multiplier = 0x2032,
    guest_physical_address = 0x2400,
    guest_link_pointer = 0x2800,
    guest_debugctl = 0x2802,
    guest_pat = 0x2804,
    guest_efer = 0x2806,
    guest_perf_global_ctrl = 0x2808,
    guest_pdpte0 = 0x280a,
    guest_pdpte1 = 0x280c,
    guest_pdpte2 = 0x280e,
    guest_pdpte3 = 0x2810,
    host_pat = 0x2c00,
    host_efer = 0x2c02,
    host_perf_global_ctrl = 0x2c04,
    ctrl_pin_based_vm_execution_controls = 0x4000,
    ctrl_processor_based_vm_execution_controls = 0x4002,
    ctrl_exception_bitmap = 0x4004,
    ctrl_pagefault_error_code_mask = 0x4006,
    ctrl_pagefault_error_code_match = 0x4008,
    ctrl_cr3_target_count = 0x400a,
    ctrl_vmexit_controls = 0x400c,
    ctrl_vmexit_msr_store_count = 0x400e,
    ctrl_vmexit_msr_load_count = 0x4010,
    ctrl_vmentry_controls = 0x4012,
    ctrl_vmentry_msr_load_count = 0x4014,
    ctrl_vmentry_interruption_information_field = 0x4016,
    ctrl_vmentry_exception_error_code = 0x4018,
    ctrl_vmentry_instruction_length = 0x401a,
    ctrl_tpr_threshold = 0x401c,
    ctrl_secondary_processor_based_vm_execution_controls = 0x401e,
    ctrl_ple_gap = 0x4020,
    ctrl_ple_window = 0x4022,
    vm_instruction_error = 0x4400,
    exit_reason = 0x4402,
    vmexit_interruption_information = 0x4404,
    vmexit_interruption_error_code = 0x4406,
    idt_vectoring_information = 0x4408,
    idt_vectoring_error_code = 0x440a,
    vmexit_instruction_length = 0x440c,
    vmexit_instruction_info = 0x440e,
    guest_es_limit = 0x4800,
    guest_cs_limit = 0x4802,
    guest_ss_limit = 0x4804,
    guest_ds_limit = 0x4806,
    guest_fs_limit = 0x4808,
    guest_gs_limit = 0x480a,
    guest_ldtr_limit = 0x480c,
    guest_tr_limit = 0x480e,
    guest_gdtr_limit = 0x4810,
    guest_idtr_limit = 0x4812,
    guest_es_access_rights = 0x4814,
    guest_cs_access_rights = 0x4816,
    guest_ss_access_rights = 0x4818,
    guest_ds_access_rights = 0x481a,
    guest_fs_access_rights = 0x481c,
    guest_gs_access_rights = 0x481e,
    guest_ldtr_access_rights = 0x4820,
    guest_tr_access_rights = 0x4822,
    guest_interruptibility_state = 0x4824,
    guest_activity_state = 0x4826,
    guest_smbase = 0x4828,
    guest_sysenter_cs = 0x482a,
    guest_vmx_preemption_timer_value = 0x482e,
    sysenter_cs = 0x4c00,
    ctrl_cr0_guest_host_mask = 0x6000,
    ctrl_cr4_guest_host_mask = 0x6002,
    ctrl_cr0_read_shadow = 0x6004,
    ctrl_cr4_read_shadow = 0x6006,
    ctrl_cr3_target_value_0 = 0x6008,
    ctrl_cr3_target_value_1 = 0x600a,
    ctrl_cr3_target_value_2 = 0x600c,
    ctrl_cr3_target_value_3 = 0x600e,
    exit_qualification = 0x6400,
    io_rcx = 0x6402,
    io_rsx = 0x6404,
    io_rdi = 0x6406,
    io_rip = 0x6408,
    exit_guest_linear_address = 0x640a,
    guest_cr0 = 0x6800,
    guest_cr3 = 0x6802,
    guest_cr4 = 0x6804,
    guest_es_base = 0x6806,
    guest_cs_base = 0x6808,
    guest_ss_base = 0x680a,
    guest_ds_base = 0x680c,
    guest_fs_base = 0x680e,
    guest_gs_base = 0x6810,
    guest_ldtr_base = 0x6812,
    guest_tr_base = 0x6814,
    guest_gdtr_base = 0x6816,
    guest_idtr_base = 0x6818,
    guest_dr7 = 0x681a,
    guest_rsp = 0x681c,
    guest_rip = 0x681e,
    guest_rflags = 0x6820,
    guest_pending_debug_exceptions = 0x6822,
    guest_sysenter_esp = 0x6824,
    guest_sysenter_eip = 0x6826,
    host_cr0 = 0x6c00,
    host_cr3 = 0x6c02,
    host_cr4 = 0x6c04,
    host_fs_base = 0x6c06,
    host_gs_base = 0x6c08,
    host_tr_base = 0x6c0a,
    host_gdtr_base = 0x6c0c,
    host_idtr_base = 0x6c0e,
    host_sysenter_esp = 0x6c10,
    host_sysenter_eip = 0x6c12,
    host_rsp = 0x6c14,
    host_rip = 0x6c16
};

#pragma pack(push, 1)

struct vmfunc_control_t {
    union {
        struct {
            uint64_t eptp_switching : 1;
            uint64_t reserved0 : 63;
        } bits;
        uint64_t raw;
    };
};
static_assert(sizeof(vmfunc_control_t) == 8, "sizeof(vmfunc_control_t)");

enum class vmentry_interrupt_type_t : uint32_t {
    external_interrupt = 0,
    reserved = 1,
    nmi = 2,
    hardware_exception = 3,
    software_interrupt = 4,
    privileged_software_exception = 5,
    software_exception = 6,
    other_event = 7
};

struct vmentry_interruption_info_t {
    union {
        struct {
            uint32_t vector : 8;
            vmentry_interrupt_type_t type : 2;
            uint32_t deliver_error_code : 1;
            uint32_t reserved : 19;
            uint32_t valid : 1;
        } bits;
        uint32_t raw;
    };
};
static_assert(sizeof(vmentry_interruption_info_t) == 4, "sizeof(vmentry_interruption_info_t)");

#pragma pack(pop)

// for improvement of instructions later: https://github.com/opnsense/src/blob/cdc5c1db54c5183add40a0a48a7692d7d4ac4a31/sys/amd64/vmm/intel/vmx_cpufunc.h#L118

static inline instruction_result_t vmclear(physical_address_t vmcs_address) {
    auto error = instruction_result_t::success;
    asm volatile("vmclear %1\n"
                 VMX_SET_ERROR_CODE
            : [error] "=r"(error) : "m"(*reinterpret_cast<uint64_t*>(&vmcs_address)) : "memory");
    return error;
}

static inline instruction_result_t vmread(field_t field, uint64_t& value) {
    auto error = instruction_result_t::success;
    asm volatile("vmread %[field], %[value]\n"
                 VMX_SET_ERROR_CODE
            : [error] "=r"(error), [value]"=m"(value) : [field]"r"(static_cast<uint64_t>(field)) : "memory");
    return error;
}

static inline instruction_result_t vmwrite(field_t field, uint64_t value) {
    auto error = instruction_result_t::success;
    asm volatile("vmwrite %2, %1\n"
                 VMX_SET_ERROR_CODE
            : [error] "=r"(error) : "r"(static_cast<uint64_t>(field)), "r"(value) : "memory");
    return error;
}

static inline instruction_result_t vmptrld(physical_address_t vmcs_address) {
    auto error = instruction_result_t::success;
    asm volatile("vmptrld %1\n"
                 VMX_SET_ERROR_CODE
            : [error] "=r"(error) : "m"(*reinterpret_cast<uint64_t*>(&vmcs_address)) : "memory");
    return error;
}

static inline instruction_result_t vmptrst(physical_address_t& vmcs_address) {
    auto error = instruction_result_t::success;
    asm volatile("vmptrst %1\n"
                 VMX_SET_ERROR_CODE
            : [error] "=r"(error), "=m"(*reinterpret_cast<uint64_t*>(&vmcs_address)) : : "memory");
    return error;
}

static inline instruction_error_t vm_instruction_error() {
    uint64_t value;
    const auto error = vmread(field_t::vm_instruction_error, value);
    // ReSharper disable once CppDFAConstantConditions
    if (error != instruction_result_t::success) {
        return instruction_error_t::failed_retrieval_of_error;
    }

    return static_cast<instruction_error_t>(value);
}

}
