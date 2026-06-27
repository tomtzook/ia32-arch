#pragma once

#include "x86/opcode.h"
#include "x86/opcode_tables.h"

namespace x86::opcode {

enum class decoded_operand_type_t : uint8_t {
    none,
    immediate_byte,
    immediate_word,
    immediate_dword,
    immediate_qword,
    instruction_displacement,
    reg,
    memory,
    memory_offset,
    memory_scaled,
    memory_scaled2,
    memory_sum,
};

struct decoded_operand_t {
    decoded_operand_type_t type;
    union {
        uint8_t i_byte;
        uint16_t i_word;
        uint32_t i_dword;
        uint64_t i_qword;
        int64_t instruct_displacement;
        register_t reg;
        struct {
            register_t base;
            ssize_t displacement;
        } mem;
        struct {
            ssize_t displacement;
        } mem_offset;
        struct {
            register_t base;
            register_t index;
            ssize_t scale;
            ssize_t displacement;
        } mem_scaled;
        struct {
            register_t index;
            ssize_t scale;
            ssize_t displacement;
        } mem_scaled2;
        struct {
            register_t reg1;
            register_t reg2;
            ssize_t displacement;
        } mem_sum;
    } value;
};

struct decoded_opcode_t {
    struct {
        bool lock;
        repeat_type_t repeat;
        segment_override_t segment_override;
        branch_hint_t branch_hint;
        bool operand_size;
        bool address_size;
        struct {
            bool present;
            bool base;
            bool index;
            bool reg;
            bool width;
        } rex;
    } prefix;

    tables::opcode_t definition;
    opcode_family_t family;
    uint8_t opcode;
    uint32_t full_opcode;
    instruction_t instruction;
    decoded_operand_t op1;
    decoded_operand_t op2;
};

enum class decode_error_t {
    success = 0,
    unsupported_hardware_mode,
    unsupported_operand_size,
    unknown_register_encoding,
    unknown_extended_register_encoding,
    unknown_operand_addressing_size,
    unknown_modrm_mod,
    unknown_operand_addressing_mode,
    opcode_definition_not_found,
    opcode_group_is_unknown,
    opcode_missing_embedded_data
};

struct decode_result_t {
    bool success;
    decode_error_t error_code;
    const void* ptr;
    decoded_opcode_t opcode;
};

decode_result_t decode(mode_t mode, const void* ptr);

}
