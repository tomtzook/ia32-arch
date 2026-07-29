#pragma once

#include "x86/types.h"
#include "x86/opcode.h"

namespace x86::opcode::tables {

enum class opcode_flag_t {
    none = 0,
    lockable,
    group,
    pair,
    stack_op
};

constexpr opcode_flag_t operator|(const opcode_flag_t lhs, const opcode_flag_t rhs) {
    return static_cast<opcode_flag_t>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

constexpr opcode_flag_t operator&(const opcode_flag_t lhs, const opcode_flag_t rhs) {
    return static_cast<opcode_flag_t>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

// see Intel SDM Vol 2 Appendix A Chapter A.2.1
enum class opaddr_t {
    A,  // A : he instruction has no ModR/M byte; the address of the operand is encoded in the instruction.
        // No base register, index register, or scaling factor can be applied (for example, far JMP (EA)).
    B,  // B : The VEX.vvvv field of the VEX prefix selects a general purpose register
    C,  // C : The reg field of the ModR/M byte selects a control register
    D,  // D : The reg field of the ModR/M byte selects a debug register
    E,  // E : A ModR/M byte follows the opcode and specifies the operand. The operand is either a general-purpose
        // register or a memory address. If it is a memory address, the address is computed from a segment register
        // and any of the following values: a base register, an index register, a scaling factor, a displacement.
    F,  // F : EFLAGS/RFLAGS Register

    G,  // G : The reg field of the ModR/M byte selects a general register
    H,  // H : The VEX.vvvv field of the VEX prefix selects a 128-bit XMM register or a 256-bit YMM register, determined
        // by operand type. For legacy SSE encodings this operand does not exist, changing the instruction to
        // destructive form
    I,  // I : Immediate data: the operand value is encoded in subsequent bytes of the instruction.
    J,  // J : The instruction contains a relative offset to be added to the instruction pointer register
    L,  // L : The upper 4 bits of the 8-bit immediate selects a 128-bit XMM register or a 256-bit YMM register, determined by operand type.
    M,  // M : The ModR/M byte may refer only to memory
    N,  // N: The R/M field of the ModR/M byte selects a packed-quadword, MMX technology register.
    O,  // O: The instruction has no ModR/M byte. The offset of the operand is coded as a word or double word
        // (depending on address size attribute) in the instruction. No base register, index register, or scaling factor
        // can be applied
    P,  // P : The reg field of the ModR/M byte selects a packed quadword MMX technology register
    Q,  // Q : A ModR/M byte follows the opcode and specifies the operand. The operand is either an MMX technology register or a memory address. If it is a memory address, the address is computed from a segment register
        // and any of the following values: a base register, an index register, a scaling factor, and a displacement
    R,  // R : The R/M field of the ModR/M byte may refer only to a general register
    S,  // S : The reg field of the ModR/M byte selects a segment register
    U,  // U : The R/M field of the ModR/M byte selects a 128-bit XMM register or a 256-bit YMM register, determined by operand type.
    V,  // V : The reg field of the ModR/M byte selects a 128-bit XMM register or a 256-bit YMM register, determined by operand type.
    W,  // W : A ModR/M byte follows the opcode and specifies the operand. The operand is either a 128-bit XMM register, a 256-bit YMM register (determined by operand type), or a memory address. If it is a memory address, the
        // address is computed from a segment register and any of the following values: a base register, an index register, a scaling factor, and a displacement.
    X,  // X : Memory addressed by the DS:rSI register pair
    Y,  // Y : Memory addressed by the ES:rDI register pair
};

// see Intel SDM Vol 2 Appendix A Chapter A.2.2
enum class opsize_t {
    none,
    a, // a : Two one-word operands in memory or two double-word operands in memory, depending on operand-size attribute
    b, // b : Byte, regardless of operand-size attribute.
    c, // c : Byte or word, depending on operand-size attribute.
    d, // d : Doubleword, regardless of operand-size attribute.
    dq,// dq : Double-quadword, regardless of operand-size attribute.
    p, // p : 32-bit, 48-bit, or 80-bit pointer, depending on operand-size attribute
    pd,// pd : 128-bit or 256-bit packed double precision floating-point data.
    pl,// pl : Quadword MMX technology register
    ps,// ps : 128-bit or 256-bit packed single-precision floating-point data.
    q, // q : Quadword, regardless of operand-size attribute
    qq,// qq : Quad-Quadword (256-bits), regardless of operand-size attribute.
    s, // s : 6-byte or 10-byte pseudo-descriptor
    sd,// sd : Scalar element of a 128-bit double precision floating data
    ss,// ss : Scalar element of a 128-bit single-precision floating data
    si,// si : Doubleword integer register
    v, // v : Word, doubleword or quadword (in 64-bit mode), depending on operand-size attribute.
    w, // w : Word, regardless of operand-size attribute
    x, // x : dq or qq based on the operand-size attribute
    y, // y : Doubleword or quadword (in 64-bit mode), depending on operand-size attribute
    z, // z : Word for 16-bit operand-size or doubleword for 32 or 64-bit operand-size.
};

enum class mem_addr_size_t {
    byte = 0,
    variable
};

enum class embedded_info_type_t {
    none,
    reg,
    reg_enc,
    memory,
    const_int,
};

struct opcode_embedded_info_t {
    embedded_info_type_t type = embedded_info_type_t::none;
    union {
        register_t reg;
        extended_register_encoding_t reg_enc;
        mem_addr_size_t mem_size;
        int i;
    } data{};

    // todo: enforce type access!
    constexpr opcode_embedded_info_t() = default;
    // ReSharper disable once CppNonExplicitConvertingConstructor
    constexpr opcode_embedded_info_t(const register_t reg) : type(embedded_info_type_t::reg), data({.reg = reg}) {}
    // ReSharper disable once CppNonExplicitConvertingConstructor
    constexpr opcode_embedded_info_t(const extended_register_encoding_t reg_enc) : type(embedded_info_type_t::reg_enc), data({.reg_enc = reg_enc}) {}
    // ReSharper disable once CppNonExplicitConvertingConstructor
    constexpr opcode_embedded_info_t(const mem_addr_size_t mem_size) : type(embedded_info_type_t::memory), data({.mem_size = mem_size}) {}
    // ReSharper disable once CppNonExplicitConvertingConstructor
    constexpr opcode_embedded_info_t(const int i) : type(embedded_info_type_t::const_int), data({.i = i}) {}
};

struct operand_t {
    opaddr_t addressing;
    opsize_t size;
    opcode_embedded_info_t embedded;
    bool exists = true;
};

struct opcode_t {
    union kind_t {
        instruction_t instruction;
        instruction_pair_t pair;
        opcode_group_t group;

        // todo: enforce type access!
        constexpr kind_t() = default;
        // ReSharper disable once CppNonExplicitConvertingConstructor
        constexpr kind_t(const instruction_t instruction) : instruction(instruction) {}
        // ReSharper disable once CppNonExplicitConvertingConstructor
        constexpr kind_t(const instruction_pair_t pair) : pair(pair) {}
        // ReSharper disable once CppNonExplicitConvertingConstructor
        constexpr kind_t(const opcode_group_t group) : group(group) {}
    };

    kind_t kind;
    operand_t operand1;
    operand_t operand2;
    opcode_flag_t flags;
    bool exists = true;

    constexpr bool is_empty() const { return !exists; }
    constexpr bool is_group() const { return (flags & opcode_flag_t::group) != opcode_flag_t::none; }
    constexpr bool is_pair() const { return (flags & opcode_flag_t::pair) != opcode_flag_t::none; }
    constexpr bool is_stack_op() const { return (flags & opcode_flag_t::stack_op) != opcode_flag_t::none; }
};

static constexpr auto table_size = 256;
extern const opcode_t table_primary[table_size];
extern const opcode_t table_extended_2byte[table_size];

static constexpr auto group_table_size = 8;
// todo: handle the reset of the groups!
extern const opcode_t table_group_1[group_table_size];
extern const opcode_t table_group_2[group_table_size];
extern const opcode_t table_group_3[group_table_size];
extern const opcode_t table_group_4[group_table_size];
extern const opcode_t table_group_5[group_table_size];
extern const opcode_t table_group_6[group_table_size];
extern const opcode_t table_group_7[group_table_size];
extern const opcode_t table_group_7_complete[64];
extern const opcode_t table_group_8[group_table_size];
extern const opcode_t table_group_9[group_table_size];
extern const opcode_t table_group_11[group_table_size];
extern const opcode_t table_group_15[group_table_size];

const opcode_t* find_opcode(opcode_family_t family, uint8_t value);
const opcode_t* find_opcode_from_group(opcode_group_t group, uint8_t value);
opcode_t merge_group_opcode(const opcode_t* original, const opcode_t* specific);

}
