
#include <stdio.h>
#include "x86/opcode_decode.h"


static void print_signed_hex(const int64_t val) {
    printf("%s0x%lx", (val < 0) ? "-" : "", (val < 0) ? -val : val);
}

static void print_op(const x86::opcode::decoded_operand_t& operand) {
    if (operand.type == x86::opcode::decoded_operand_type_t::none) {
        return;
    }
    switch (operand.type) {
        case x86::opcode::decoded_operand_type_t::immediate_byte:
            printf("0x%x", operand.value.i_byte);
            break;
        case x86::opcode::decoded_operand_type_t::immediate_word:
            printf("0x%x", operand.value.i_word);
            break;
        case x86::opcode::decoded_operand_type_t::immediate_dword:
            printf("0x%x", operand.value.i_dword);
            break;
        case x86::opcode::decoded_operand_type_t::immediate_qword:
            printf("0x%llx", operand.value.i_qword);
            break;
        case x86::opcode::decoded_operand_type_t::instruction_displacement:
            printf("rel ");
            print_signed_hex(operand.value.instruct_displacement);
            break;
        case x86::opcode::decoded_operand_type_t::reg:
            printf("%s", x86::opcode::get_register_name(operand.value.reg));
            break;
        case x86::opcode::decoded_operand_type_t::memory:
            printf("[%s + 0x%lx]",
                x86::opcode::get_register_name(operand.value.mem.base),
                operand.value.mem.displacement);
            break;
        case x86::opcode::decoded_operand_type_t::memory_offset:
            printf("[%ld]", operand.value.mem_offset.displacement);
            break;
        case x86::opcode::decoded_operand_type_t::memory_scaled:
            printf("[%s + (%s * %ld) + %ld]",
                x86::opcode::get_register_name(operand.value.mem_scaled.base),
                x86::opcode::get_register_name(operand.value.mem_scaled.index),
                operand.value.mem_scaled.scale,
                operand.value.mem_scaled.displacement);
            break;
        case x86::opcode::decoded_operand_type_t::memory_scaled2:
            printf("[(%s * %ld) + %ld]",
                x86::opcode::get_register_name(operand.value.mem_scaled2.index),
                operand.value.mem_scaled2.scale,
                operand.value.mem_scaled2.displacement);
            break;
        case x86::opcode::decoded_operand_type_t::memory_sum:
            printf("[%s + %s + %ld]",
                x86::opcode::get_register_name(operand.value.mem_sum.reg1),
                x86::opcode::get_register_name(operand.value.mem_sum.reg2),
                operand.value.mem_sum.displacement);
            break;
        default:
            break;
    }
}

static void print(const x86::opcode::decoded_opcode_t& opcode) {
    if (opcode.prefix.lock) {
        printf("LOCK ");
    }

    printf("%s ", x86::opcode::get_instruction_mnemonic(opcode.definition.kind.instruction));
    print_op(opcode.op1);
    if (opcode.op1.type != x86::opcode::decoded_operand_type_t::none && opcode.op2.type != x86::opcode::decoded_operand_type_t::none) {
        printf(", ");
    }
    print_op(opcode.op2);
    printf("\n");
}

int main() {
    /*const uint8_t opcodes[] = {
        0x48, 0xC7, 0xC0, 0x02, 0x00, 0x00, 0x00, // mov rax, 2
        0xFF, 0xE0, // jmp rax
        0x66, 0xFF, 0xE0, // jmp ax
        0xB0, 0x02, // mov al, 2
        0xB4, 0x02, // mov ah, 2
        0xD1, 0xC0, // rol eax, 1
    };*/
    const uint8_t opcodes[] = {
        0x51,                                           // push    rcx
        0x48, 0x8D, 0x4C, 0x24, 0x30,                   // lea     rcx, [rsp+8+arg_20]
        0x48, 0x83, 0xEC, 0x20,                         // sub     rsp, 20h
        0xE8, 0x11, 0x82, 0x00, 0x00,                   // call    asm_cpu_store_registers
        0x48, 0x83, 0xC4, 0x20,                         // add     rsp, 20h
        0x58,                                           // pop     rax
        0x48, 0x89, 0x41, 0x10,                         // mov     [rcx+10h], rax
        0x48, 0x89, 0xC8,                               // mov     rax, rcx
        0xB9, 0x04, 0x00, 0x00, 0x00,                   // mov     ecx, 4          ; unsigned __int64
        0xBA, 0x00, 0x00, 0x00, 0x00,                   // mov     edx, 0          ; unsigned __int64
        0x4C, 0x8B, 0x04, 0x24,                         // mov     r8, [rsp+0]     ; unsigned __int64
        0x4C, 0x0F, 0xB7, 0x4C, 0x24, 0x08,             // movzx   r9, [rsp+arg_0] ; unsigned __int16
        0x4C, 0x8B, 0x54, 0x24, 0x10,                   // mov     r10, [rsp+arg_8]
        0x4C, 0x8B, 0x5C, 0x24, 0x18,                   // mov     r11, [rsp+arg_10]
        0x4C, 0x0F, 0xB7, 0x64, 0x24, 0x20,             // movzx   r12, [rsp+arg_18]
        0x4C, 0x89, 0x80, 0x88, 0x00, 0x00, 0x00,       // mov     [rax+88h], r8
        0x66, 0x44, 0x89, 0x88, 0x90, 0x00, 0x00, 0x00, // mov     [rax+90h], r9w
        0x4C, 0x89, 0x50, 0x70,                         // mov     [rax+70h], r10
        0x4C, 0x89, 0x98, 0x80, 0x00, 0x00, 0x00,       // mov     [rax+80h], r11
        0x66, 0x44, 0x89, 0xA0, 0x9A, 0x00, 0x00, 0x00, // mov     [rax+9Ah], r12w
        0x48, 0x83, 0xEC, 0x20,                         // sub     rsp, 20h
        0xE8, 0xC6, 0xF4, 0xFF, 0xFF,                   // call    idt_handler
        0x48, 0x83, 0xC4, 0x20,                         // add     rsp, 20h
        0x48, 0x8D, 0x4C, 0x24, 0x30,                   // lea     rcx, [rsp+arg_28]
        0x48, 0x83, 0xEC, 0x30,                         // sub     rsp, 30h
        0xE9, 0x1D, 0x82, 0x00, 0x00,                   // jmp     asm_cpu_load_registers
    };

    const auto* ptr = opcodes;
    const auto* end_ptr = ptr + sizeof(opcodes);
    while (ptr < end_ptr) {
        const auto res = x86::opcode::decode(x86::opcode::mode_t::long_mode, ptr);
        if (res.success) {
            print(res.opcode);
            ptr = static_cast<const uint8_t*>(res.ptr);
        } else {
            printf("Failed to decode: code=0x%x\n", res.error_code);
            break;
        }
    }

    return 0;
}