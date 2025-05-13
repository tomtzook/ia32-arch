#pragma once


#define	VMX_SET_ERROR_CODE \
	"	jnc 1f;"						\
	"	mov $1, %[error];"	/* CF: error = 1 */		\
	"	jmp 3f;"						\
	"1:	jnz 2f;"						\
	"	mov $2, %[error];"	/* ZF: error = 2 */		\
	"	jmp 3f;"						\
	"2:	mov $0, %[error];"					\
	"3:"

namespace x86::vmx {

enum class instruction_result_t : uint8_t{
    success = 0,
    vm_fail_invalid = 1,
    vm_fail_valid = 2
};

// VM Instruction Error Numbers [SDM 3 30.4 P1221]

enum class instruction_error_t : uint16_t {
	vmcall_in_vmx_root = 1,
	vmclear_invalid_address = 2,
	vmclear_with_vmxon_pointer = 3,
	vmlaunch_non_clear_vmcs = 4,
	vmresume_non_launched_vmcs = 5,
	vmresult_afer_vmxoff = 6,
	vmentry_invalid_control = 7,
	vmentry_invalid_host_state = 8,
	vmptrld_invalid_address = 9,
	vmptrld_with_vmxon_pointer = 10,
	vmptrld_with_incorrect_vmcs_revision = 11,
	vmreadwrite_unsupported_component = 12,
	vmwrite_to_readonly = 13,
	vmxon_in_vmx_root = 15,
	vmentry_invalid_execvmcs_pointer = 16,
	vmentry_with_non_launched_execvmcs = 17,
	vmentry_with_execvmcs_ptr_not_vmxon = 18,
	vmcall_non_clear_vmcs = 19,
	vmcall_invalid_vmexit_control = 20,
	vmcall_incorrect_mseg_revision = 22,
	vmxoff_dual_monitor_smi_smm = 23,
	vmcall_invalid_smm_monitor_features = 24,
	vmentry_invalid_vmexec_control_in_execvmcs = 25,
	vmentry_events_blocked_by_mov_ss = 26,
	invalid_op_to_invept_invpid = 28,

	failed_retrieval_of_error = 0xff
};

}
