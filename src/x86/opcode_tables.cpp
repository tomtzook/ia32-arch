
#include "x86/opcode_tables.h"

namespace x86::opcode::tables {

const opcode_t* find_opcode(const opcode_family_t family, const uint8_t value) {
    if (value >= table_size) {
        return nullptr;
    }

    const opcode_t* def;
    switch (family) {
        case opcode_family_t::primary:
            def = &table_primary[value];
            break;
        case opcode_family_t::extended_2byte:
            def = &table_extended_2byte[value];
            break;
        case opcode_family_t::extended_3byte_1:
        case opcode_family_t::extended_3byte_2:
        case opcode_family_t::fpu:
        case opcode_family_t::vex_2byte:
        case opcode_family_t::vex_3byte:
        case opcode_family_t::evex:
        default:
            return nullptr;
    }

    if (def == nullptr || !def->exists) {
        return nullptr;
    }

    return def;
}

const opcode_t* find_opcode_from_group(const opcode_group_t group, const uint8_t value) {
    if (value >= group_table_size) {
        return nullptr;
    }

    const opcode_t* def = nullptr;
    switch (group) {
        case opcode_group_t::group1:
            def = &table_group_1[value];
            break;
        case opcode_group_t::group2:
            def = &table_group_2[value];
            break;
        case opcode_group_t::group3:
            def = &table_group_3[value];
            break;
        case opcode_group_t::group4:
            def = &table_group_4[value];
            break;
        case opcode_group_t::group5:
            def = &table_group_5[value];
            break;
        case opcode_group_t::group6:
            def = &table_group_6[value];
            break;
        case opcode_group_t::group7:
            def = &table_group_7[value];
            break;
        case opcode_group_t::group8:
            def = &table_group_8[value];
            break;
        case opcode_group_t::group9:
            def = &table_group_9[value];
            break;
        case opcode_group_t::group11:
            def = &table_group_11[value];
            break;
        case opcode_group_t::group15:
            def = &table_group_15[value];
            break;
    }

    if (def == nullptr || !def->exists) {
        return nullptr;
    }

    return def;
}

opcode_t merge_group_opcode(const opcode_t* original, const opcode_t* specific) {
    // merge definitions based on what the group contains
    opcode_t def = *original;
    if (!def.operand1.exists && specific->operand1.exists) {
        def = *specific;
    } else {
        def.kind = specific->kind;
    }

    return def;
}

constexpr operand_t no_operand = { opaddr_t::A, opsize_t::a, {}, false };
constexpr instruction_t no_instruction = instruction_t::aaa;
constexpr opcode_t no_opcode = { no_instruction, no_operand, no_operand, opcode_flag_t::none, false };

// Opcode maps are in Intel SDM Vol 2 Appendix A.3
// also found here https://www.sandpile.org

// todo: create tables based on system states and such (one for long, one for protected, one for sse, etc)

const opcode_t table_primary[256] = {
    // 0x00 - 0x0F
    { instruction_t::add,  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::add,  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::add,  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::add,  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::add,  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::add,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { instruction_t::push, { opaddr_t::O, opsize_t::w, { register_t::es } }, no_operand,                             opcode_flag_t::stack_op },
    { instruction_t::pop,  { opaddr_t::O, opsize_t::w, { register_t::es } }, no_operand,                             opcode_flag_t::stack_op },
    { instruction_t::or_,   { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::or_,   { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::or_,   { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::or_,   { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::or_,   { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::or_,   { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { instruction_t::push, { opaddr_t::O, opsize_t::w,  { register_t::cs } }, no_operand,                             opcode_flag_t::stack_op },
    no_opcode, // 2byte escape

    // 0x10 - 0x1F
    { instruction_t::adc,  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::adc,  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::adc,  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::adc,  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::adc,  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::adc,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { instruction_t::push, { opaddr_t::O, opsize_t::w, { register_t::ss } }, no_operand,                             opcode_flag_t::stack_op },
    { instruction_t::pop,  { opaddr_t::O, opsize_t::w, { register_t::ss } }, no_operand,                             opcode_flag_t::none },
    { instruction_t::sbb,  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::sbb,  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::sbb,  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::sbb,  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::sbb,  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::sbb,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { instruction_t::push, { opaddr_t::O, opsize_t::w, { register_t::ds } }, no_operand,                             opcode_flag_t::stack_op },
    { instruction_t::pop,  { opaddr_t::O, opsize_t::w, { register_t::ds } }, no_operand,                             opcode_flag_t::none },

    // 0x20 - 0x2F
    { instruction_t::and_,  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::and_,  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::and_,  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::and_,  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::and_,  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::and_,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    no_opcode, // seg es prefix
    { instruction_t::daa,  no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::sub,  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::sub,  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::sub,  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::sub,  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::sub,  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::sub,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    no_opcode, // seg cs prefix OR branch hint prefix
    { instruction_t::das,  no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0x30 - 0x3F
    { instruction_t::xor_,  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::xor_,  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::xor_,  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::xor_,  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::xor_,  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::xor_,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    no_opcode, // seg ss prefix
    { instruction_t::aaa,  no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::cmp,  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::cmp,  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmp,  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::cmp,  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmp,  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::cmp,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    no_opcode, // seg cs prefix OR branch taken prefix
    { instruction_t::aas,  no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0x40 - 0x4F (REX prefixes in 64-bit mode)
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,

    // 0x50 - 0x5F
    { instruction_t::push, { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rax } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::push, { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rcx } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::push, { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rdx } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::push, { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rbx } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::push, { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rsp } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::push, { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rbp } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::push, { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rsi } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::push, { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rdi } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::pop,  { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rax } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::pop,  { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rcx } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::pop,  { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rdx } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::pop,  { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rbx } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::pop,  { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rsp } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::pop,  { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rbp } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::pop,  { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rsi } }, no_operand, opcode_flag_t::stack_op },
    { instruction_t::pop,  { opaddr_t::O, opsize_t::v, { extended_register_encoding_t::rdi } }, no_operand, opcode_flag_t::stack_op },

    // 0x60 - 0x6F
    { instruction_t::pusha, no_operand,                           no_operand,                             opcode_flag_t::stack_op },
    { instruction_t::popa,  no_operand,                           no_operand,                             opcode_flag_t::none },
    { instruction_t::bound, no_operand,                   no_operand,                             opcode_flag_t::none },
    { instruction_t::movsxd,{ opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::d }, opcode_flag_t::none },
    no_opcode, // seg fs prefix
    no_opcode, // seg gs prefix
    no_opcode, // opsize prefix
    no_opcode, // addrsize prefix
    { instruction_t::push, { opaddr_t::I, opsize_t::z }, no_operand,                             opcode_flag_t::stack_op },
    { instruction_t::imul, { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::push, { opaddr_t::I, opsize_t::b }, no_operand,                             opcode_flag_t::stack_op },
    { instruction_t::imul, { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::insb, { opaddr_t::Y, opsize_t::b }, { opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, opcode_flag_t::none },
    { instruction_t::insw, { opaddr_t::Y, opsize_t::v }, { opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, opcode_flag_t::none },
    { instruction_t::outsb,{ opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, { opaddr_t::X, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::outsw,{ opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rax } }, { opaddr_t::X, opsize_t::v }, opcode_flag_t::none },

    // 0x70 - 0x7F
    { instruction_t::jo,   { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jno,  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jb,   { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jnb,  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jz,   { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jnz,  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jbe,  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jnbe, { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::js,   { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jns,  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jp,   { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jnp,  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jl,   { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jnl,  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jle,  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { instruction_t::jnle, { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },

    // 0x80 - 0x8F
    { opcode_group_t::group1, { opaddr_t::E, opsize_t::b }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable | opcode_flag_t::group }, // grp1
    { opcode_group_t::group1, { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::lockable | opcode_flag_t::group }, // grp1
    { opcode_group_t::group1, { opaddr_t::E, opsize_t::b }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable | opcode_flag_t::group }, // grp1
    { opcode_group_t::group1, { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable | opcode_flag_t::group }, // grp1
    { instruction_t::test, { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::test, { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::xchg, { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::xchg, { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::mov,  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::E, opsize_t::v }, { opaddr_t::S, opsize_t::w }, opcode_flag_t::none },
    { instruction_t::lea,  { opaddr_t::G, opsize_t::v }, { opaddr_t::M, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::S, opsize_t::w }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { opcode_group_t::group1,{ opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::none }, // grp1a

    // 0x90 - 0x9F
    { instruction_t::nop,  no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::xchg, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rcx } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::xchg, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdx } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::xchg, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbx } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::xchg, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsp } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::xchg, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbp } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::xchg, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsi } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::xchg, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsi } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::cbw,  no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::cwd,  no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::callf, no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_pair_t::wait_or_fwait, no_operand,                            no_operand,                             opcode_flag_t::pair },
    { instruction_t::pushf,{ opaddr_t::F, opsize_t::v }, no_operand,                             opcode_flag_t::stack_op },
    { instruction_t::popf, { opaddr_t::F, opsize_t::v }, no_operand,                             opcode_flag_t::stack_op },
    { instruction_t::sahf, no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::lahf, no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0xA0 - 0xAF
    { instruction_t::mov,  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::A, opsize_t::b, { mem_addr_size_t::byte } }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::A, opsize_t::v, { mem_addr_size_t::variable } }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::b, { mem_addr_size_t::byte } }, { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::v, { mem_addr_size_t::variable } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::movsb,{ opaddr_t::Y, opsize_t::b }, { opaddr_t::X, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::movsw,{ opaddr_t::Y, opsize_t::v }, { opaddr_t::X, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmpsb,{ opaddr_t::X, opsize_t::b }, { opaddr_t::Y, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::cmpsw,{ opaddr_t::X, opsize_t::v }, { opaddr_t::Y, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::test, { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::test, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { instruction_t::stosb,{ opaddr_t::Y, opsize_t::b }, { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::stosw,{ opaddr_t::Y, opsize_t::v }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::lodsb,{ opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::X, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::lodsw,{ opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::X, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::scasb,{ opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::Y, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::scasw,{ opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::Y, opsize_t::v }, opcode_flag_t::none },

    // 0xB0 - 0xBF
    { instruction_t::mov,  { opaddr_t::A, opsize_t::b, { register_t::al } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::b, { register_t::cl } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::b, { register_t::dl } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::b, { register_t::bl } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::b, { register_t::ah } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::b, { register_t::ch } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::b, { register_t::dh } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::b, { register_t::bh } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rcx } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdx } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbx } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsp } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbp } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsi } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::mov,  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdi } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },

    // 0xC0 - 0xCF
    { opcode_group_t::group2, { opaddr_t::E, opsize_t::b }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::group },
    { opcode_group_t::group2, { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::group },
    { instruction_t::ret,  { opaddr_t::I, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { instruction_t::ret,  no_operand,                            no_operand,                             opcode_flag_t::none },
    no_opcode, // escape vex 3byte
    no_opcode, // escape vex 3byte
    { opcode_group_t::group11,{ opaddr_t::E, opsize_t::b }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::group },
    { opcode_group_t::group11,{ opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::group },
    { instruction_t::enter,{ opaddr_t::I, opsize_t::w }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::leave,no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::retf, { opaddr_t::I, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { instruction_t::retf, no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::int3, no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::int_,  { opaddr_t::I, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::into, no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::iret, no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0xD0 - 0xD7
    { opcode_group_t::group2, { opaddr_t::E, opsize_t::b }, {opaddr_t::A, opsize_t::b, { 1 } }, opcode_flag_t::group },
    { opcode_group_t::group2, { opaddr_t::E, opsize_t::v }, {opaddr_t::A, opsize_t::b, { 1 } }, opcode_flag_t::group },
    { opcode_group_t::group2, { opaddr_t::E, opsize_t::b }, { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rcx } }, opcode_flag_t::group },
    { opcode_group_t::group2, { opaddr_t::E, opsize_t::v }, {opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rcx } }, opcode_flag_t::group },
    { instruction_t::aam,  { opaddr_t::I, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::aad,  { opaddr_t::I, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::salc, no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::xlat, no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0xD8 - 0xDF (FPU Escapes)
    no_opcode, // escape fpu
    no_opcode, // escape fpu
    no_opcode, // escape fpu
    no_opcode, // escape fpu
    no_opcode, // escape fpu
    no_opcode, // escape fpu
    no_opcode, // escape fpu
    no_opcode, // escape fpu

    // 0xE0 - 0xEF
    { instruction_t::loopne, { opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::loope,  { opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::loop,   { opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jcxz,   { opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::in,     { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::in,     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::out,    { opaddr_t::I, opsize_t::b }, { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::out,    { opaddr_t::I, opsize_t::b }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::call,   { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jmp,    { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jmpf,   no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::jmp,    { opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::in,     { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, opcode_flag_t::none },
    { instruction_t::in,     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, opcode_flag_t::none },
    { instruction_t::out,    { opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { instruction_t::out,    { opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },

    // 0xF0 - 0xFF
    no_opcode, // lock prefix
    { instruction_t::icebp,  no_operand,                          no_operand,                             opcode_flag_t::none },
    no_opcode, // repne prefix
    no_opcode, // rep prefix
    { instruction_t::hlt,    no_operand,                          no_operand,                             opcode_flag_t::none },
    { instruction_t::cmc,    no_operand,                          no_operand,                             opcode_flag_t::none },
    { opcode_group_t::group3,   { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::group },
    { opcode_group_t::group3,   { opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::group },
    { instruction_t::clc,    no_operand,                          no_operand,                             opcode_flag_t::none },
    { instruction_t::stc,    no_operand,                          no_operand,                             opcode_flag_t::none },
    { instruction_t::cli,    no_operand,                          no_operand,                             opcode_flag_t::none },
    { instruction_t::sti,    no_operand,                          no_operand,                             opcode_flag_t::none },
    { instruction_t::cld,    no_operand,                          no_operand,                             opcode_flag_t::none },
    { instruction_t::std,    no_operand,                          no_operand,                             opcode_flag_t::none },
    { opcode_group_t::group4,   no_operand,                          no_operand,                             opcode_flag_t::group },
    { opcode_group_t::group5,   no_operand,                          no_operand,                             opcode_flag_t::group }
};


const opcode_t table_extended_2byte[256] = {
    // 0x00 - 0x0F
    { opcode_group_t::group6,      no_operand,                          no_operand,                             opcode_flag_t::group },
    { opcode_group_t::group7,      no_operand,                          no_operand,                             opcode_flag_t::group },
    { instruction_t::lar,       { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::w }, opcode_flag_t::none },
    { instruction_t::lsl,       { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::w }, opcode_flag_t::none },
    no_opcode,
    { instruction_pair_t::loadall_or_syscall, no_operand,                   no_operand,                             opcode_flag_t::pair }, // 0x05 (SYSCALL in 64-bit)
    { instruction_t::clts,      no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::sysret,    no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::invd,      no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::wbinvd,    no_operand,                            no_operand,                             opcode_flag_t::none },
    no_opcode,
    { instruction_t::ud2,       no_operand,                            no_operand,                             opcode_flag_t::none }, // 0x0B
    no_opcode,
    { opcode_group_t::group_nop,   { opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::group }, // 0x0D (Hint NOPs / Prefetch)
    { instruction_t::femms,     no_operand,                            no_operand,                             opcode_flag_t::none },
    no_opcode, // 3dnow escape

    // 0x10 - 0x1F (SSE / SIMD Move instructions)
    { instruction_pair_t::movups_or_movupd,  { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::movups_or_movupd,  { opaddr_t::W, opsize_t::ps }, { opaddr_t::V, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::movlps_or_movlpd,  { opaddr_t::V, opsize_t::q  }, { opaddr_t::M, opsize_t::q  }, opcode_flag_t::pair },
    { instruction_pair_t::movlps_or_movlpd,  { opaddr_t::M, opsize_t::q  }, { opaddr_t::V, opsize_t::q  }, opcode_flag_t::pair },
    { instruction_pair_t::unpcklps_or_unpcklpd, { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::unpckhps_or_unpckhpd, { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::movhps_or_movhpd,  { opaddr_t::V, opsize_t::q  }, { opaddr_t::M, opsize_t::q  }, opcode_flag_t::pair },
    { instruction_pair_t::movhps_or_movhpd,  { opaddr_t::M, opsize_t::q  }, { opaddr_t::V, opsize_t::q  }, opcode_flag_t::pair },
    { opcode_group_t::group16,     { opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::group }, // 0x18 (Prefetch group)
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    { instruction_t::nop,       { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none }, // 0x1F (Multi-byte NOP)

    // 0x20 - 0x2F (Control/Debug Register Moves & SIMD conversions)
    { instruction_t::mov,       { opaddr_t::C, opsize_t::y }, { opaddr_t::G, opsize_t::y }, opcode_flag_t::none }, // CR moves
    { instruction_t::mov,       { opaddr_t::D, opsize_t::y }, { opaddr_t::G, opsize_t::y }, opcode_flag_t::none }, // DR moves
    { instruction_t::mov,       { opaddr_t::G, opsize_t::y }, { opaddr_t::C, opsize_t::y }, opcode_flag_t::none },
    { instruction_t::mov,       { opaddr_t::G, opsize_t::y }, { opaddr_t::D, opsize_t::y }, opcode_flag_t::none },
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    { instruction_t::movaps,    { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none }, // 0x28
    { instruction_t::movaps,    { opaddr_t::W, opsize_t::ps }, { opaddr_t::V, opsize_t::ps }, opcode_flag_t::none },
    { instruction_pair_t::cvtpi2ps_or_cvttps2pi, { opaddr_t::V, opsize_t::ps }, { opaddr_t::Q, opsize_t::q }, opcode_flag_t::pair },
    { instruction_pair_t::movntps_or_movntpd,    { opaddr_t::M, opsize_t::ps }, { opaddr_t::V, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::cvtps2pi_or_cvtpi2pd,  { opaddr_t::Q, opsize_t::q  }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::cvttps2pi_or_cvttpd2pi, { opaddr_t::Q, opsize_t::q  }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::ucomiss_or_ucomisd,    { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::comiss_or_comisd,      { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },

    // 0x30 - 0x3F (System instructions & Multi-byte extensions escapes)
    { instruction_t::wrmsr,     no_operand,                            no_operand,                             opcode_flag_t::none }, // 0x30
    { instruction_t::rdtsc,     no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::rdmsr,     no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::rdpmc,     no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::sysenter,  no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::sysexit,   no_operand,                            no_operand,                             opcode_flag_t::none },
    no_opcode,
    { instruction_t::getsec,    no_operand,                            no_operand,                             opcode_flag_t::none },
    no_opcode, // 3 byte escape 1
    no_opcode,
    no_opcode, // 3 byte escape 2
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,

    // 0x40 - 0x4F (CMOVcc operations)
    { instruction_t::cmovo,     { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovno,    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovb,     { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovnb,    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovz,     { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovnz,    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovbe,    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovnbe,   { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovs,     { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none }, // 0x48
    { instruction_t::cmovns,    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovp,     { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovnp,    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovl,     { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovnl,    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovle,    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::cmovnle,   { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },

    // 0x50 - 0x5F (SIMD Floating-Point Math)
    { instruction_pair_t::movmskps_or_movmskpd, { opaddr_t::G, opsize_t::v }, { opaddr_t::V, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::sqrtps_or_sqrtpd,     { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::rsqrtps_or_rsqrtss,   { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::rcpps_or_rcpss,       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::andps_or_andpd,       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::andnps_or_andnpd,     { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::orps_or_orpd,         { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::xorps_or_xorpd,       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::addps_or_addpd,       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair }, // 0x58
    { instruction_pair_t::mulps_or_mulpd,       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::cvtps2pd_or_cvtpd2ps, { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::cvtdq2ps_or_cvtps2dq, { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::subps_or_subpd,       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::minps_or_minpd,       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::divps_or_divpd,       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_pair_t::maxps_or_maxpd,       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },

    // 0x60 - 0x6F (MMX / SSE Packed Integer Operations)
    { instruction_t::punpclbw,  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::d },  opcode_flag_t::none },
    { instruction_t::punpclwd,  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::d },  opcode_flag_t::none },
    { instruction_t::punpcldq,  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::d },  opcode_flag_t::none },
    { instruction_t::packsswb,  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { instruction_t::pcmpgtb,   { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { instruction_t::pcmpgtw,   { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { instruction_t::pcmpgtd,   { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { instruction_t::packuswb,  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { instruction_t::punpchbw,  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none }, // 0x68
    { instruction_t::punpchwd,  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { instruction_t::punpchdq,  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { instruction_t::packssdw,  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { instruction_t::punpchqdq, { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { instruction_t::punpclqdq, { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { instruction_pair_t::movd_or_movq, { opaddr_t::P, opsize_t::d }, { opaddr_t::Q, opsize_t::d }, opcode_flag_t::pair },
    { instruction_pair_t::movdqa_or_movdqu, { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::pair },

    // 0x70 - 0x7F (SIMD Shuffles and Logic shifts)
    { instruction_pair_t::pshufw_or_pshufdb, { opaddr_t::Q, opsize_t::q }, { opaddr_t::N, opsize_t::q }, opcode_flag_t::pair },
    { opcode_group_t::group12,     { opaddr_t::N, opsize_t::q }, no_operand,                             opcode_flag_t::group }, // Shift Group (psllw, etc)
    { opcode_group_t::group13,     { opaddr_t::N, opsize_t::q }, no_operand,                             opcode_flag_t::group }, // Shift Group (psraw, etc)
    { opcode_group_t::group14,     { opaddr_t::N, opsize_t::q }, no_operand,                             opcode_flag_t::group }, // Shift Group (psllq, etc)
    { instruction_t::pcmpeqb,   { opaddr_t::P, opsize_t::q }, { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { instruction_t::pcmpeqw,   { opaddr_t::P, opsize_t::q }, { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { instruction_t::pcmpeqd,   { opaddr_t::P, opsize_t::q }, { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { instruction_t::emms,      no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::vmread,    { opaddr_t::E, opsize_t::q }, { opaddr_t::G, opsize_t::q },  opcode_flag_t::none }, // 0x78 (VMX)
    { instruction_t::vmwrite,   { opaddr_t::G, opsize_t::q }, { opaddr_t::E, opsize_t::q },  opcode_flag_t::none }, // 0x79 (VMX)
    no_opcode,
    no_opcode,
    { instruction_pair_t::haddpd_or_haddps,  { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::pair },
    { instruction_pair_t::hsubpd_or_hsubps,  { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::pair },
    { instruction_pair_t::movd_or_movq, { opaddr_t::Q, opsize_t::d }, { opaddr_t::P, opsize_t::d }, opcode_flag_t::pair },
    { instruction_pair_t::movdqa_or_movdqu, { opaddr_t::W, opsize_t::dq }, { opaddr_t::V, opsize_t::dq }, opcode_flag_t::pair },

    // 0x80 - 0x8F (Jcc near branch targets - 16/32-bit offsets)
    { instruction_t::jo,        { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jno,       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jb,        { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jnb,       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jz,        { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jnz,       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jbe,       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jnbe,      { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::js,        { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none }, // 0x88
    { instruction_t::jns,       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jp,        { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jnp,       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jl,        { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jnl,       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jle,       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { instruction_t::jnle,      { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },

    // 0x90 - 0x9F (SETcc operations)
    { instruction_t::seto,      { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setno,     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setb,      { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setnb,     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setz,      { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setnz,     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setbe,     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setnbe,    { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::sets,      { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none }, // 0x98
    { instruction_t::setns,     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setp,      { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setnp,     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setl,      { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setnl,     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setle,     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { instruction_t::setnle,    { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },

    // 0xA0 - 0xAF (Segment pointer pushes/pops, CPUID, bit scanning operations)
    { instruction_t::push,      { opaddr_t::O, opsize_t::w, { register_t::fs } }, no_operand,                             opcode_flag_t::stack_op },
    { instruction_t::pop,       { opaddr_t::O, opsize_t::w, { register_t::fs } }, no_operand,                             opcode_flag_t::none },
    { instruction_t::cpuid,     no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::bt,        { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::shld,      { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none }, // 3rd operand implicit imm byte
    { instruction_t::shld,      { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none }, // 3rd operand implicit CL
    no_opcode,
    no_opcode,
    { instruction_t::push,      { opaddr_t::O, opsize_t::w, { register_t::gs } }, no_operand,                             opcode_flag_t::stack_op }, // 0xA8
    { instruction_t::pop,       { opaddr_t::O, opsize_t::w, { register_t::gs } }, no_operand,                             opcode_flag_t::none },
    { instruction_t::rsm,       no_operand,                            no_operand,                             opcode_flag_t::none },
    { instruction_t::bts,       { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::shrd,      { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::shrd,      { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { opcode_group_t::group15,     no_operand, no_operand,                             opcode_flag_t::group }, // FXSAVE, FXRSTOR, LFENCE...
    { instruction_t::imul,      { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },

    // 0xB0 - 0xBF (CMPXCHG, Data conversions, Bit Tests)
    { instruction_t::cmpxchg,   { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::cmpxchg,   { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::lss,       { opaddr_t::G, opsize_t::v }, { opaddr_t::M, opsize_t::p }, opcode_flag_t::none },
    { instruction_t::btr,       { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::lfs,       { opaddr_t::G, opsize_t::v }, { opaddr_t::M, opsize_t::p }, opcode_flag_t::none },
    { instruction_t::lgs,       { opaddr_t::G, opsize_t::v }, { opaddr_t::M, opsize_t::p }, opcode_flag_t::none },
    { instruction_t::movzbl,    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::movzx,    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::w }, opcode_flag_t::none },
    no_opcode,
    { instruction_t::ud1,       no_operand,                            no_operand,                             opcode_flag_t::none },
    { opcode_group_t::group8,      no_operand,                            no_operand,                           opcode_flag_t::lockable | opcode_flag_t::group }, // Bit group extensions
    { instruction_t::btc,       { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_t::bsf,       { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::bsr,       { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::movsbl,    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::movswl,    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::w }, opcode_flag_t::none },

    // 0xC0 - 0xCF (XADD, SIMD permutations, and multi-byte additions)
    { instruction_t::xadd,      { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::xadd,      { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { instruction_pair_t::cmpps_or_cmppd,    { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { instruction_t::movnti,    { opaddr_t::M, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { instruction_t::pinsrw,    { opaddr_t::P, opsize_t::q }, { opaddr_t::E, opsize_t::w }, opcode_flag_t::none },
    { instruction_t::pextrw,    { opaddr_t::G, opsize_t::v }, { opaddr_t::N, opsize_t::q }, opcode_flag_t::none },
    { instruction_pair_t::shufps_or_shufpd,  { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::pair },
    { opcode_group_t::group9,      no_operand, no_operand,                             opcode_flag_t::lockable | opcode_flag_t::group }, // 0xC7 (CMPXCHG8B/16B)
    { instruction_t::bswap,     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, no_operand,                             opcode_flag_t::none }, // 0xC8
    { instruction_t::bswap,     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rcx } }, no_operand,                             opcode_flag_t::none },
    { instruction_t::bswap,     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdx } }, no_operand,                             opcode_flag_t::none },
    { instruction_t::bswap,     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbx } }, no_operand,                             opcode_flag_t::none },
    { instruction_t::bswap,     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsp } }, no_operand,                             opcode_flag_t::none },
    { instruction_t::bswap,     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbp } }, no_operand,                             opcode_flag_t::none },
    { instruction_t::bswap,     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsi } }, no_operand,                             opcode_flag_t::none },
    { instruction_t::bswap,     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdi } }, no_operand,                             opcode_flag_t::none },

    // 0xD0 - 0xDF (SSE / MMX Packed Math Continued)
    { instruction_pair_t::addsubpd_or_addsubps, { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::pair },
    { instruction_t::pand,      { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pandn,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::por,       { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pxor,      { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::paddb,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::paddw,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::paddd,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pmaxub,    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none }, // 0xD8
    { instruction_t::pminub,    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pmulhuw,   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pmulhw,    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_pair_t::cvttpd2dq_or_cvtpi2pd, { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::pair },
    { instruction_pair_t::pinsrd_or_pinsrq,      { opaddr_t::V, opsize_t::dq }, { opaddr_t::E, opsize_t::d  }, opcode_flag_t::pair },
    { instruction_pair_t::pextrd_or_pextrq,      { opaddr_t::E, opsize_t::d  }, { opaddr_t::V, opsize_t::dq }, opcode_flag_t::pair },
    { instruction_t::pmuludq,   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },

    // 0xE0 - 0xEF
    { instruction_t::pavgb,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::psraw,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::psrad,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pminsw,    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pmaxsw,    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pmaddwd,   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::psadbw,    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_pair_t::maskmovq_or_maskmovdqu, { opaddr_t::P, opsize_t::q  }, { opaddr_t::N, opsize_t::q  }, opcode_flag_t::pair },
    { instruction_t::psubb,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none }, // 0xE8
    { instruction_t::psubw,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::psubd,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pextrw,    { opaddr_t::G, opsize_t::v  }, { opaddr_t::V, opsize_t::dq }, opcode_flag_t::none },
    { instruction_t::pmullw,    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pmulildq,  { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { instruction_t::movdq2q,   { opaddr_t::P, opsize_t::q  }, { opaddr_t::W, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::movq2dq,   { opaddr_t::V, opsize_t::dq }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },

    // 0xF0 - 0xFF
    no_opcode,
    { instruction_t::psllw,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pslld,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::psllq,     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pmuludq,   { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { instruction_t::pmaddubw,  { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { instruction_t::psubusb,   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::psubusw,   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pminsd,    { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none }, // 0xF8
    { instruction_t::pmaxsd,    { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { instruction_t::pextrw,    { opaddr_t::E, opsize_t::w  }, { opaddr_t::V, opsize_t::dq }, opcode_flag_t::none },
    { instruction_t::paddusb,   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::paddusw,   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { instruction_t::pmaxud,    { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { instruction_t::pminud,    { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    no_opcode,
};
// Group 1: Immediate Extensions (Opcodes: 80, 81, 82, 83)
// Note: Operand 2 size is dynamically resolved ('v' or 'b') depending on the primary opcode
const opcode_t table_group_1[8] = {
    { instruction_t::add, { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { instruction_t::or_,  { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { instruction_t::adc, { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { instruction_t::sbb, { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { instruction_t::and_, { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { instruction_t::sub, { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { instruction_t::xor_, { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { instruction_t::txt, { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::none } // CMP semantics
};

// Group 2: Shift & Rotate Extensions (Opcodes: C0, C1, D0, D1, D2, D3)
const opcode_t table_group_2[8] = {
    { instruction_t::rol, { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { instruction_t::ror, { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { instruction_t::rcl, { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { instruction_t::rcr, { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { instruction_t::shl, { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { instruction_t::shr, { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { instruction_t::sal, { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { instruction_t::sar, { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none }
};

// Group 3: Unary Operations / Multiply / Divide (Opcodes: F6, F7)
const opcode_t table_group_3[8] = {
    { instruction_t::test,    { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::v },    opcode_flag_t::none },
    no_opcode,
    { instruction_t::not_,     { opaddr_t::E, opsize_t::v }, no_operand,                            opcode_flag_t::lockable },
    { instruction_t::neg,     { opaddr_t::E, opsize_t::v }, no_operand,                            opcode_flag_t::lockable },
    { instruction_t::mul,     { opaddr_t::E, opsize_t::v }, no_operand,                            opcode_flag_t::none },
    { instruction_t::imul,    { opaddr_t::E, opsize_t::v }, no_operand,                            opcode_flag_t::none },
    { instruction_t::div,     { opaddr_t::E, opsize_t::v }, no_operand,                            opcode_flag_t::none },
    { instruction_t::idiv,    { opaddr_t::E, opsize_t::v }, no_operand,                            opcode_flag_t::none }
};

// Group 4: Increment / Decrement 8-bit (Opcode: FE)
const opcode_t table_group_4[8] = {
    { instruction_t::inc,     { opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::lockable },
    { instruction_t::dec,     { opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::lockable },
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
};

// Group 5: Near & Far Indirect Jumps / Calls / Pushes (Opcode: FF)
const opcode_t table_group_5[8] = {
    { instruction_t::inc,     { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::lockable },
    { instruction_t::dec,     { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::lockable },
    { instruction_t::call,    { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none }, // Near
    { instruction_t::call,    { opaddr_t::E, opsize_t::p }, no_operand, opcode_flag_t::none }, // Far
    { instruction_t::jmp,     { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none }, // Near
    { instruction_t::jmp,     { opaddr_t::E, opsize_t::p }, no_operand, opcode_flag_t::none }, // Far
    { instruction_t::push,    { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::stack_op },
    no_opcode,
};

// Group 6: System Tables Architecture (Opcode: 0F 00)
const opcode_t table_group_6[8] = {
    { instruction_t::sldt,    { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none },
    { instruction_t::str,     { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none },
    { instruction_t::lldt,    { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none },
    { instruction_t::ltr,     { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none },
    { instruction_t::verr,    { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none },
    { instruction_t::verw,    { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none },
    no_opcode,
    no_opcode,
};

// Group 7: Descriptor Tables & System States (Opcode: 0F 01)
const opcode_t table_group_7[8] = {
    { instruction_t::sgdt,    { opaddr_t::M, opsize_t::p }, no_operand, opcode_flag_t::none },
    { instruction_t::sidt,    { opaddr_t::M, opsize_t::p }, no_operand, opcode_flag_t::none },
    { instruction_t::lgdt,    { opaddr_t::M, opsize_t::p }, no_operand, opcode_flag_t::none },
    { instruction_t::lidt,    { opaddr_t::M, opsize_t::p }, no_operand, opcode_flag_t::none },
    { instruction_t::smsw,    { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none }, // Maps inside MSW slot
    no_opcode,
    { instruction_t::invlpg,  { opaddr_t::M, opsize_t::b }, no_operand, opcode_flag_t::none },
    { no_instruction,no_operand,                            no_operand, opcode_flag_t::none } // Escapes to direct register matchers (SWAPGS, etc) // todo:
};

const opcode_t table_group_7_complete[64] = {
    // 0xc0 -> 0xff
    no_opcode, // c0
    { instruction_t::vmcall,no_operand, no_operand, opcode_flag_t::none }, // c1
    { instruction_t::vmlaunch, no_operand, no_operand, opcode_flag_t::none }, // c2
    { instruction_t::vmresume, no_operand, no_operand, opcode_flag_t::none }, // c3
    { instruction_t::vmxoff, no_operand, no_operand, opcode_flag_t::none }, // c4
    no_opcode, // c5
    no_opcode, // c6
    no_opcode, // c7
    { instruction_t::monitor, no_operand, no_operand, opcode_flag_t::none }, // c8
    { instruction_t::mwait, no_operand, no_operand, opcode_flag_t::none }, // c9
    { instruction_t::clac, no_operand, no_operand, opcode_flag_t::none }, // ca
    { instruction_t::stac, no_operand, no_operand, opcode_flag_t::none }, // cb
    no_opcode, // cc
    no_opcode, // cd
    no_opcode, // ce
    no_opcode, // cf
    { instruction_t::xgetbv, no_operand, no_operand, opcode_flag_t::none }, // d0
    { instruction_t::xsetbv, no_operand, no_operand, opcode_flag_t::none }, // d1
    no_opcode, // d2
    no_opcode, // d3
    { instruction_t::vmfunc, no_operand, no_operand, opcode_flag_t::none }, // d4
    { instruction_t::xend, no_operand, no_operand, opcode_flag_t::none }, // d5
    no_opcode, // d6
    no_opcode, // d7
    no_opcode, // d8
    no_opcode, // d9
    no_opcode, // da
    no_opcode, // db
    no_opcode, // dc
    no_opcode, // dd
    no_opcode, // de
    no_opcode, // df
    no_opcode, // e0
    no_opcode, // e1
    no_opcode, // e2
    no_opcode, // e3
    no_opcode, // e4
    no_opcode, // e5
    no_opcode, // e6
    no_opcode, // e7
    no_opcode, // e8
    no_opcode, // e9
    no_opcode, // ea
    no_opcode, // eb
    no_opcode, // ec
    no_opcode, // ed
    no_opcode, // ee
    no_opcode, // ef
    no_opcode, // f0
    no_opcode, // f1
    no_opcode, // f2
    no_opcode, // f3
    no_opcode, // f4
    no_opcode, // f5
    no_opcode, // f6
    no_opcode, // f7
    { instruction_t::swapgs, no_operand, no_operand, opcode_flag_t::none }, // f8
    { instruction_t::rdtscp, no_operand, no_operand, opcode_flag_t::none }, // f9
    no_opcode, // fa
    no_opcode, // fb
    no_opcode, // fc
    no_opcode, // fd
    no_opcode, // fe
    no_opcode, // ff
};

// Group 8: Bit Tests with Immediates (Opcode: 0F BA)
const opcode_t table_group_8[8] = {
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    { instruction_t::bt,      { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { instruction_t::btc,     { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::btr,     { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable },
    { instruction_t::bts,     { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable }
};

// Group 9: Compare and Exchange (Opcode: 0F C7)
const opcode_t table_group_9[8] = {
    no_opcode,
    { instruction_t::cmpxchg8b, { opaddr_t::M, opsize_t::q }, no_operand, opcode_flag_t::lockable }, // Resolves to 16B if REX.W=1
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    { instruction_t::vmptrld,   { opaddr_t::M, opsize_t::q }, no_operand, opcode_flag_t::none },
    { instruction_t::vmptrst,   { opaddr_t::M, opsize_t::q }, no_operand, opcode_flag_t::none }
};

// Group 11: Immediate Move Block (Opcodes: C6, C7)
const opcode_t table_group_11[8] = {
    { instruction_t::mov,     { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
};

// Group 15: Memory Serialization / Tracking / Fences (Opcode: 0F AE)
const opcode_t table_group_15[8] = {
    { instruction_t::fxsave,     { opaddr_t::M, opsize_t::none }, no_operand, opcode_flag_t::none },
    { instruction_t::fxrstor,    { opaddr_t::M, opsize_t::none }, no_operand, opcode_flag_t::none },
    { instruction_t::ldmxcsr,    { opaddr_t::M, opsize_t::d    }, no_operand, opcode_flag_t::none },
    { instruction_t::stmxcsr,    { opaddr_t::M, opsize_t::d    }, no_operand, opcode_flag_t::none },
    { instruction_t::xsave,      { opaddr_t::M, opsize_t::none }, no_operand, opcode_flag_t::none }, // LFENCE space if Mod == 11
    { instruction_t::xrstor,     { opaddr_t::M, opsize_t::none }, no_operand, opcode_flag_t::none }, // MFENCE space if Mod == 11
    { instruction_t::clflush,    { opaddr_t::M, opsize_t::b    }, no_operand, opcode_flag_t::none }, // SFENCE space if Mod == 11
    { instruction_t::clflushopt, { opaddr_t::M, opsize_t::b    }, no_operand, opcode_flag_t::none }
};

}
