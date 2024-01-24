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

enum class error_t {
    success = 0,
    vm_fail_invalid = 1,
    vm_fail_valid = 2
};

}
