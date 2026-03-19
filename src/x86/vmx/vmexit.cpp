
#include "x86/vmx/vmexit.h"

namespace x86::vmx {

const char* exit_reason_str(const exit_reason_t reason) {
    switch (reason) {
        case exit_reason_t::exception_or_nmi: return "exception_or_nmi";
        case exit_reason_t::external_interrupt: return "external_interrupt";
        case exit_reason_t::triple_fault: return "triple_fault";
        case exit_reason_t::init_signal: return "init_signal";
        case exit_reason_t::startup_ipi: return "startup_ipi";
        case exit_reason_t::io_smm_interrupt: return "io_smm_interrupt";
        case exit_reason_t::other_smi: return "other_smi";
        case exit_reason_t::interrupt_window: return "interrupt_window";
        case exit_reason_t::nmi_windows: return "nmi_windows";
        case exit_reason_t::task_switch: return "task_switch";
        case exit_reason_t::cpuid: return "cpuid";
        case exit_reason_t::getsec: return "getsec";
        case exit_reason_t::hlt: return "hlt";
        case exit_reason_t::invd: return "invd";
        case exit_reason_t::invlpg: return "invlpg";
        case exit_reason_t::rdpmc: return "rdpmc";
        case exit_reason_t::rdtsc: return "rdtsc";
        case exit_reason_t::rsm: return "rsm";
        case exit_reason_t::vmcall: return "vmcall";
        case exit_reason_t::vmclear: return "vmclear";
        case exit_reason_t::vmlaunch: return "vmlaunch";
        case exit_reason_t::vmptrld: return "vmptrld";
        case exit_reason_t::vmptrst: return "vmptrst";
        case exit_reason_t::vmread: return "vmread";
        case exit_reason_t::vmresume: return "vmresume";
        case exit_reason_t::vmwrite: return "vmwrite";
        case exit_reason_t::vmxoff: return "vmxoff";
        case exit_reason_t::vmxon: return "vmxon";
        case exit_reason_t::cr_access: return "cr_access";
        case exit_reason_t::mov_dr: return "mov_dr";
        case exit_reason_t::io: return "io";
        case exit_reason_t::rdmsr: return "rdmsr";
        case exit_reason_t::wrmsr: return "wrmsr";
        case exit_reason_t::vmentry_invalid_guest_state: return "vmentry_invalid_guest_state";
        case exit_reason_t::vmentry_msr_load_failure: return "vmentry_msr_load_failure";
        case exit_reason_t::mwait: return "mwait";
        case exit_reason_t::monitor_trap_flag: return "monitor_trap_flag";
        case exit_reason_t::monitor: return "monitor";
        case exit_reason_t::pause: return "pause";
        case exit_reason_t::vmentry_machine_check: return "vmentry_machine_check";
        case exit_reason_t::tpr_below_threshold: return "tpr_below_threshold";
        case exit_reason_t::apic_access: return "apic_access";
        case exit_reason_t::virtualized_eoi: return "virtualized_eoi";
        case exit_reason_t::access_gdtr_idtr: return "access_gdtr_idtr";
        case exit_reason_t::access_ldtr_tr: return "access_ldtr_tr";
        case exit_reason_t::ept_violation: return "ept_violation";
        case exit_reason_t::ept_misconfiguration: return "ept_misconfiguration";
        case exit_reason_t::invept: return "invept";
        case exit_reason_t::rdtscp: return "rdtscp";
        case exit_reason_t::vmx_preemption_timer_expired: return "vmx_preemption_timer_expired";
        case exit_reason_t::invvpid: return "invvpid";
        case exit_reason_t::wbinvd: return "wbinvd";
        case exit_reason_t::xsetbv: return "xsetbv";
        case exit_reason_t::apic_write: return "apic_write";
        case exit_reason_t::rdrand: return "rdrand";
        case exit_reason_t::invpcid: return "invpcid";
        case exit_reason_t::vmfunc: return "vmfunc";
        case exit_reason_t::encls: return "encls";
        case exit_reason_t::rdseed: return "rdseed";
        case exit_reason_t::page_mod_log_full: return "page_mod_log_full";
        case exit_reason_t::xsaves: return "xsaves";
        case exit_reason_t::xrstors: return "xrstors";
        default: return "";
    }
}

}
