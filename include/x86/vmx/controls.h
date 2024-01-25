#pragma once

#include "x86/common.h"
#include "x86/msr.h"
#include "x86/vmx/vmcs.h"

namespace x86::vmx {

#pragma pack(push, 1)

template<
        typename _allowed_msr,
        typename _allowed_true_msr,
        field_t _vmcs_field,
        typename meta::enable_if<
                msr::is_msr_def<_allowed_msr>::value,
                bool>::type = 0,
        typename meta::enable_if<
                msr::is_msr_def<_allowed_true_msr>::value,
                bool>::type = 0>
struct _vmx_controls {
    using allowed_msr = _allowed_msr;
    using allowed_true_msr = _allowed_true_msr;
    static constexpr field_t vmcs_field = _vmcs_field;
};

// [SDM 3 24.6.1 P1061 "Table 24-5"]
struct pin_based_exec_controls_t :
        public _vmx_controls<
                x86::msr::ia32_vmx_pinbased_ctls_t,
                x86::msr::ia32_vmx_true_pinbased_ctls_t,
                field_t::ctrl_pin_based_vm_execution_controls> {
    union {
        struct {
            uint32_t external_interrupt_exiting : 1;
            uint32_t unused0 : 2;
            uint32_t nmi_exiting : 1;
            uint32_t unused1 : 1;
            uint32_t virtual_nmis : 1;
            uint32_t activate_preemption_time : 1;
            uint32_t process_posted_interrupts : 1;
            uint32_t unused2 : 21;
        } bits;
        uint32_t raw;
    };
};
static_assert(sizeof(pin_based_exec_controls_t) == 4, "sizeof(pin_based_exec_controls_t)");

// [SDM 3 24.6.2 P1061 "Table 24-6"]
struct processor_based_exec_controls_t  :
        public _vmx_controls<
                x86::msr::ia32_vmx_procbased_ctls_t,
                x86::msr::ia32_vmx_true_procbased_ctls_t,
                field_t::ctrl_processor_based_vm_execution_controls> {
    union {
        struct {
            uint32_t unused0 : 2;
            uint32_t interrupt_window_existing : 1;
            uint32_t use_tsc_offsetting : 1;
            uint32_t unused1 : 3;
            uint32_t hlt_exiting : 1;
            uint32_t unused2 : 1;
            uint32_t invlpg_exiting : 1;
            uint32_t mwait_existing : 1;
            uint32_t rdpmc_existing : 1;
            uint32_t rdtsc_exiting : 1;
            uint32_t unused3 : 2;
            uint32_t cr3_load_existing : 1;
            uint32_t cr3_store_exiting : 1;
            uint32_t unused4 : 2;
            uint32_t cr8_load_exiting : 1;
            uint32_t cr8_store_exiting : 1;
            uint32_t use_tpr_shadow : 1;
            uint32_t nmi_window_exiting : 1;
            uint32_t mov_dr_exiting : 1;
            uint32_t unconditional_io_exiting : 1;
            uint32_t use_io_bitmaps : 1;
            uint32_t unused5 : 1;
            uint32_t monitor_trap_flag : 1;
            uint32_t use_msr_bitmaps : 1;
            uint32_t monitor_exiting : 1;
            uint32_t pause_exiting : 1;
            uint32_t activate_secondary_controls : 1;
        } bits;
        uint32_t raw;
    };
};
static_assert(sizeof(processor_based_exec_controls_t) == 4, "sizeof(processor_based_exec_controls_t)");

// [SDM 3 24.6.2 P1063 "Table 24-7"]
struct secondary_processor_based_exec_controls_t :
        public _vmx_controls<
                x86::msr::ia32_vmx_procbased_ctls2_t,
                x86::msr::ia32_vmx_procbased_ctls2_t,
                field_t::ctrl_secondary_processor_based_vm_execution_controls> {
    union {
        struct {
            uint32_t virtualize_apic_accesses : 1;
            uint32_t enable_ept : 1;
            uint32_t descriptor_table_exiting : 1;
            uint32_t enable_rdtscp : 1;
            uint32_t virtualize_x2apic : 1;
            uint32_t enable_vpid : 1;
            uint32_t wbinvd_exiting : 1;
            uint32_t unrestricted_guest : 1;
            uint32_t apic_register_virtualization : 1;
            uint32_t virtual_interrupt_delivery : 1;
            uint32_t pause_loop_exiting : 1;
            uint32_t rdrand_exiting : 1;
            uint32_t enable_invpcid : 1;
            uint32_t enable_vm_functions : 1;
            uint32_t vmcs_shadowing : 1;
            uint32_t enable_encls_exiting : 1;
            uint32_t rdseed_exiting : 1;
            uint32_t enable_pml : 1;
            uint32_t ept_violation_ve : 1;
            uint32_t conceal_vmx_non_root_from_ipt : 1;
            uint32_t enable_xsaves_xstors : 1;
            uint32_t mode_based_execution_control : 1;
            uint32_t use_tsc_scaling : 1;
            uint32_t unused0 : 6;
        } bits;
        uint32_t raw;
    };
};
static_assert(sizeof(secondary_processor_based_exec_controls_t) == 4, "sizeof(secondary_processor_based_exec_controls_t)");

// [SDM 3 24.7.1 P1069 "Table 24-10"]
struct vmexit_controls_t :
        public _vmx_controls<
                x86::msr::ia32_vmx_exit_ctls_t,
                x86::msr::ia32_vmx_true_exit_ctls_t,
                field_t::ctrl_vmexit_controls> {
    union {
        struct {
            uint32_t unused0 : 2;
            uint32_t save_debug_controls : 1;
            uint32_t unused1 : 6;
            uint32_t host_address_space_size : 1;
            uint32_t unused2 : 2;
            uint32_t load_pref_glob : 1;
            uint32_t unused3 : 2;
            uint32_t acknowledge_interrupt_on_exit : 1;
            uint32_t unused4 : 2;
            uint32_t save_pat : 1;
            uint32_t load_pat : 1;
            uint32_t save_efer : 1;
            uint32_t load_efer : 1;
            uint32_t save_preemption_timer_value : 1;
            uint32_t clear_bndcfgs : 1;
            uint32_t conceal_exits_from_ipt : 1;
            uint32_t unused5 : 7;
        } bits;
        uint32_t raw;
    };
};
static_assert(sizeof(vmexit_controls_t) == 4, "sizeof(vmexit_controls_t)");

// [SDM 3 24.8.1 P1071 "Table 24-12"]
struct vmentery_controls_t :
        public _vmx_controls<
                x86::msr::ia32_vmx_entry_ctls_t,
                x86::msr::ia32_vmx_true_entry_ctls_t,
                field_t::ctrl_vmentry_controls> {
    union {
        struct {
            uint32_t unused0 : 2;
            uint32_t load_debug_controls : 1;
            uint32_t unused1 : 6;
            uint32_t ia32e_mode_guest : 1;
            uint32_t entry_to_smm : 1;
            uint32_t deactivate_dual_monitor : 1;
            uint32_t unused2 : 1;
            uint32_t load_pre_global_ctrl : 1;
            uint32_t load_pat : 1;
            uint32_t load_efer : 1;
            uint32_t load_bndcfgs : 1;
            uint32_t conceal_entries_from_ipt : 1;
            uint32_t unused3 : 14;
        } bits;
        uint32_t raw;
    };
};
static_assert(sizeof(vmentery_controls_t) == 4, "sizeof(vmentery_controls_t)");

#pragma pack(pop)

template<typename controls>
static inline controls adjust_vm_controls(const controls& wanted_controls) noexcept {
    // [SDM 3 A.3.1]
    // bits allowed0: MSR bit x = 0 -> VMCS bit allowed 0
    // bits allowed1: MSR bit x = 1 -> VMCS bit allowed 1
    controls copy{};
    copy.raw = wanted_controls.raw;

    auto allowed = x86::read<typename controls::allowed_true_msr>();
    copy.raw &= allowed.bits.allowed1;
    copy.raw |= allowed.bits.allowed0;

    return copy;
}

template<typename controls>
static inline bool are_vm_controls_supported(const controls& wanted_controls) noexcept {
    // [SDM 3 A.3.1]
    // bits allowed0: MSR bit x = 0 -> VMCS bit allowed 0
    // bits allowed1: MSR bit x = 1 -> VMCS bit allowed 1
    auto allowed = x86::read<typename controls::allowed_true_msr>();
    auto allowed_mask = ~allowed.bits.allowed0 & allowed.bits.allowed1;
    return (wanted_controls.raw & allowed_mask) == wanted_controls.raw;
}

}
