
#include "x86/types.h"
#include "x86/opcode.h"

namespace x86::opcode::def {

opcode_embedded_info_t::opcode_embedded_info_t(const register_t reg)
    : type(embedded_info_type_t::reg)
    , data({ .reg = reg })
{}

opcode_embedded_info_t::opcode_embedded_info_t(const extended_register_encoding_t reg_enc)
    : type(embedded_info_type_t::reg_enc)
    , data({ .reg_enc = reg_enc })
{}

opcode_embedded_info_t::opcode_embedded_info_t(const mem_addr_size_t mem_size)
    : type(embedded_info_type_t::memory)
    , data({ .mem_size = mem_size })
{}

opcode_embedded_info_t::opcode_embedded_info_t(const int i)
    : type(embedded_info_type_t::const_int)
    , data({ .i = i })
{}

constexpr operand_t no_operand = { opaddr_t::A, opsize_t::a, {}, false };
constexpr opcode_t no_opcode = { nullptr, no_operand, no_operand, opcode_flag_t::none, false };

// Opcode maps are in Intel SDM Vol 2 Appendix A.3
// also found here https://www.sandpile.org

const opcode_t table_primary[256] = {
    // 0x00 - 0x0F
    { "ADD",  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "ADD",  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "ADD",  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "ADD",  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "ADD",  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "ADD",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "PUSH", { opaddr_t::O, opsize_t::w, { register_t::es } }, no_operand,                             opcode_flag_t::none },
    { "POP",  { opaddr_t::O, opsize_t::w, { register_t::es } }, no_operand,                             opcode_flag_t::none },
    { "OR",   { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "OR",   { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "OR",   { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "OR",   { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "OR",   { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "OR",   { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "PUSH", { opaddr_t::O, opsize_t::w,  { register_t::cs } }, no_operand,                             opcode_flag_t::none },
    { "ESCAPE_2BYTE", no_operand,                    no_operand,                             opcode_flag_t::none },

    // 0x10 - 0x1F
    { "ADC",  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "ADC",  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "ADC",  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "ADC",  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "ADC",  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "ADC",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "PUSH", { opaddr_t::O, opsize_t::w, { register_t::ss } }, no_operand,                             opcode_flag_t::none },
    { "POP",  { opaddr_t::O, opsize_t::w, { register_t::ss } }, no_operand,                             opcode_flag_t::none },
    { "SBB",  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "SBB",  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "SBB",  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "SBB",  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "SBB",  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "SBB",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "PUSH", { opaddr_t::O, opsize_t::w, { register_t::ds } }, no_operand,                             opcode_flag_t::none },
    { "POP",  { opaddr_t::O, opsize_t::w, { register_t::ds } }, no_operand,                             opcode_flag_t::none },

    // 0x20 - 0x2F
    { "AND",  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "AND",  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "AND",  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "AND",  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "AND",  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "AND",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "SEG_ES",no_operand,                           no_operand,                             opcode_flag_t::none },
    { "DAA",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "SUB",  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "SUB",  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "SUB",  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "SUB",  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "SUB",  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "SUB",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "REG_CS_OR_BR_NOT_TAKEN", no_operand,          no_operand,                             opcode_flag_t::none },
    { "DAS",  no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0x30 - 0x3F
    { "XOR",  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "XOR",  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "XOR",  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "XOR",  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "XOR",  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "XOR",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "SEG_SS",no_operand,                           no_operand,                             opcode_flag_t::none },
    { "AAA",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "CMP",  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::none },
    { "CMP",  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "CMP",  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::none },
    { "CMP",  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMP",  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "CMP",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "SEG_DS_OR_BR_TAKEN", no_operand,              no_operand,                             opcode_flag_t::none },
    { "AAS",  no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0x40 - 0x4F (REX prefixes in 64-bit mode)
    { "REX",     no_operand, no_operand, opcode_flag_t::none }, { "REX_B",    no_operand, no_operand, opcode_flag_t::none },
    { "REX_X",    no_operand, no_operand, opcode_flag_t::none }, { "REX_XB",   no_operand, no_operand, opcode_flag_t::none },
    { "REX_R",    no_operand, no_operand, opcode_flag_t::none }, { "REX_RB",   no_operand, no_operand, opcode_flag_t::none },
    { "REX_RX",   no_operand, no_operand, opcode_flag_t::none }, { "REX_RXB",  no_operand, no_operand, opcode_flag_t::none },
    { "REX_W",    no_operand, no_operand, opcode_flag_t::none }, { "REX_WB",   no_operand, no_operand, opcode_flag_t::none },
    { "REX_WX",   no_operand, no_operand, opcode_flag_t::none }, { "REX_WXB",  no_operand, no_operand, opcode_flag_t::none },
    { "REX_WR",   no_operand, no_operand, opcode_flag_t::none }, { "REX_WRB",  no_operand, no_operand, opcode_flag_t::none },
    { "REX_WRX",  no_operand, no_operand, opcode_flag_t::none }, { "REX_WRXB", no_operand, no_operand, opcode_flag_t::none },

    // 0x50 - 0x5F
    { "PUSH", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, no_operand, opcode_flag_t::none },
    { "PUSH", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rcx } }, no_operand, opcode_flag_t::none },
    { "PUSH", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdx } }, no_operand, opcode_flag_t::none },
    { "PUSH", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbx } }, no_operand, opcode_flag_t::none },
    { "PUSH", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsp } }, no_operand, opcode_flag_t::none },
    { "PUSH", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbp } }, no_operand, opcode_flag_t::none },
    { "PUSH", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsi } }, no_operand, opcode_flag_t::none },
    { "PUSH", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdi } }, no_operand, opcode_flag_t::none },
    { "POP",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, no_operand, opcode_flag_t::none },
    { "POP",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rcx } }, no_operand, opcode_flag_t::none },
    { "POP",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdx } }, no_operand, opcode_flag_t::none },
    { "POP",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbx } }, no_operand, opcode_flag_t::none },
    { "POP",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsp } }, no_operand, opcode_flag_t::none },
    { "POP",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbp } }, no_operand, opcode_flag_t::none },
    { "POP",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsi } }, no_operand, opcode_flag_t::none },
    { "POP",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdi } }, no_operand, opcode_flag_t::none },

    // 0x60 - 0x6F
    { "PUSHA", no_operand,                           no_operand,                             opcode_flag_t::none },
    { "POPA",  no_operand,                           no_operand,                             opcode_flag_t::none },
    { "BOUND_OR_EVEX", no_operand,                   no_operand,                             opcode_flag_t::none },
    { "MOVSXD",{ opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::d }, opcode_flag_t::none },
    { "SEG_FS",no_operand,                           no_operand,                             opcode_flag_t::none },
    { "SEG_GS",no_operand,                           no_operand,                             opcode_flag_t::none },
    { "PREFIX_OPERAND_SIZE", no_operand,              no_operand,                             opcode_flag_t::none },
    { "PREFIX_ADDRESS_SIZE", no_operand,              no_operand,                             opcode_flag_t::none },
    { "PUSH", { opaddr_t::I, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "IMUL", { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "PUSH", { opaddr_t::I, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "IMUL", { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "INSB", { opaddr_t::Y, opsize_t::b }, { opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, opcode_flag_t::none },
    { "INSW", { opaddr_t::Y, opsize_t::v }, { opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, opcode_flag_t::none },
    { "OUTSB",{ opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, { opaddr_t::X, opsize_t::b }, opcode_flag_t::none },
    { "OUTSW",{ opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rax } }, { opaddr_t::X, opsize_t::v }, opcode_flag_t::none },

    // 0x70 - 0x7F
    { "JO",   { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNO",  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JB",   { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNB",  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JZ",   { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNZ",  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JBE",  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNBE", { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JS",   { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNS",  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JP",   { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNP",  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JL",   { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNL",  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JLE",  { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNLE", { opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },

    // 0x80 - 0x8F
    { "GRP1", { opaddr_t::E, opsize_t::b }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable | opcode_flag_t::group },
    { "GRP1", { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::lockable | opcode_flag_t::group },
    { "GRP1", { opaddr_t::E, opsize_t::b }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable | opcode_flag_t::group },
    { "GRP1", { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable | opcode_flag_t::group },
    { "TEST", { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::none },
    { "TEST", { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "XCHG", { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "XCHG", { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "MOV",  { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::G, opsize_t::b }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::E, opsize_t::v }, { opaddr_t::S, opsize_t::w }, opcode_flag_t::none },
    { "LEA",  { opaddr_t::G, opsize_t::v }, { opaddr_t::M, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::S, opsize_t::w }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "GRP1A",{ opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::none },

    // 0x90 - 0x9F
    { "NOP",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "XCHG", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rcx } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "XCHG", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdx } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "XCHG", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbx } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "XCHG", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsp } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "XCHG", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbp } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "XCHG", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsi } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "XCHG", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsi } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "CBW",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "CWD",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "CALLF",no_operand,                            no_operand,                             opcode_flag_t::none },
    { "WAIT", no_operand,                            no_operand,                             opcode_flag_t::none },
    { "PUSHF",{ opaddr_t::F, opsize_t::v }, no_operand,                             opcode_flag_t::none },
    { "POPF", { opaddr_t::F, opsize_t::v }, no_operand,                             opcode_flag_t::none },
    { "SAHF", no_operand,                            no_operand,                             opcode_flag_t::none },
    { "LAHF", no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0xA0 - 0xAF
    { "MOV",  { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::A, opsize_t::b, { mem_addr_size_t::byte } }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::A, opsize_t::v, { mem_addr_size_t::variable } }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::b, { mem_addr_size_t::byte } }, { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::v, { mem_addr_size_t::variable } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "MOVSB",{ opaddr_t::Y, opsize_t::b }, { opaddr_t::X, opsize_t::b }, opcode_flag_t::none },
    { "MOVSW",{ opaddr_t::Y, opsize_t::v }, { opaddr_t::X, opsize_t::v }, opcode_flag_t::none },
    { "CMPSB",{ opaddr_t::X, opsize_t::b }, { opaddr_t::Y, opsize_t::b }, opcode_flag_t::none },
    { "CMPSW",{ opaddr_t::X, opsize_t::v }, { opaddr_t::Y, opsize_t::v }, opcode_flag_t::none },
    { "TEST", { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "TEST", { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "STOSB",{ opaddr_t::Y, opsize_t::b }, { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "STOSW",{ opaddr_t::Y, opsize_t::v }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "LODSB",{ opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::X, opsize_t::b }, opcode_flag_t::none },
    { "LODSW",{ opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::X, opsize_t::v }, opcode_flag_t::none },
    { "SCASB",{ opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::Y, opsize_t::b }, opcode_flag_t::none },
    { "SCASW",{ opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::Y, opsize_t::v }, opcode_flag_t::none },

    // 0xB0 - 0xBF
    { "MOV",  { opaddr_t::A, opsize_t::b, { register_t::al } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::b, { register_t::cl } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::b, { register_t::dl } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::b, { register_t::bl } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::b, { register_t::ah } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::b, { register_t::ch } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::b, { register_t::dh } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::b, { register_t::bh } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rcx } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdx } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbx } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsp } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbp } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsi } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdi } }, { opaddr_t::I, opsize_t::v }, opcode_flag_t::none },

    // 0xC0 - 0xCF
    { "GRP2", { opaddr_t::E, opsize_t::b }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none | opcode_flag_t::group },
    { "GRP2", { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none | opcode_flag_t::group },
    { "RET",  { opaddr_t::I, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "RET",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "ESCAPE_VEX_3BYTE", no_operand,                no_operand,                             opcode_flag_t::none }, // 0xC4
    { "ESCAPE_VEX_2BYTE", no_operand,                no_operand,                             opcode_flag_t::none }, // 0xC5
    { "GRP11",{ opaddr_t::E, opsize_t::b }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none | opcode_flag_t::group },
    { "GRP11",{ opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none | opcode_flag_t::group },
    { "ENTER",{ opaddr_t::I, opsize_t::w }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none | opcode_flag_t::group },
    { "LEAVE",no_operand,                            no_operand,                             opcode_flag_t::none },
    { "RETF", { opaddr_t::I, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "RETF", no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INT3", no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INT",  { opaddr_t::I, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "INTO", no_operand,                            no_operand,                             opcode_flag_t::none },
    { "IRET", no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0xD0 - 0xD7
    { "GRP2", { opaddr_t::E, opsize_t::b }, {opaddr_t::A, opsize_t::b, { 1 } }, opcode_flag_t::none | opcode_flag_t::group },
    { "GRP2", { opaddr_t::E, opsize_t::v }, {opaddr_t::A, opsize_t::b, { 1 } }, opcode_flag_t::none | opcode_flag_t::group },
    { "GRP2", { opaddr_t::E, opsize_t::b }, { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rcx } }, opcode_flag_t::none | opcode_flag_t::group },
    { "GRP2", { opaddr_t::E, opsize_t::v }, {opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rcx } }, opcode_flag_t::none | opcode_flag_t::group },
    { "AAM",  { opaddr_t::I, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "AAD",  { opaddr_t::I, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SALC", no_operand,                            no_operand,                             opcode_flag_t::none },
    { "XLAT", no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0xD8 - 0xDF (FPU Escapes)
    { "ESCAPE_FPU", { opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },

    // 0xE0 - 0xEF
    { "LOOPNE", { opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "LOOPE",  { opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "LOOP",   { opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "jCXZ",   { opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "IN",     { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "IN",     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "OUT",    { opaddr_t::I, opsize_t::b }, { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "OUT",    { opaddr_t::I, opsize_t::b }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "CALL",   { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JMP",    { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JMPF",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "JMP",    { opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "IN",     { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, { opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, opcode_flag_t::none },
    { "IN",     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, { opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, opcode_flag_t::none },
    { "OUT",    { opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, { opaddr_t::A, opsize_t::b, { extended_register_encoding_t::rax } }, opcode_flag_t::none },
    { "OUT",    { opaddr_t::A, opsize_t::w, { extended_register_encoding_t::rdx } }, { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, opcode_flag_t::none },

    // 0xF0 - 0xFF
    { "PREFIX_LOCK", no_operand,                     no_operand,                             opcode_flag_t::none },
    { "ICEBP",  no_operand,                          no_operand,                             opcode_flag_t::none },
    { "PREFIX_REPNE", no_operand,                    no_operand,                             opcode_flag_t::none },
    { "PREFIX_REP", no_operand,                      no_operand,                             opcode_flag_t::none },
    { "HLT",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "CMC",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "GRP3",   { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group },
    { "GRP3",   { opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group },
    { "CLC",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "STC",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "CLI",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "STI",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "CLD",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "STD",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "GRP4",   no_operand,                          no_operand,                             opcode_flag_t::none | opcode_flag_t::group },
    { "GRP5",   no_operand,                          no_operand,                             opcode_flag_t::none | opcode_flag_t::group }
};

const opcode_t table_extended_2byte[256] = {
    // 0x00 - 0x0F
    { "GRP6",      no_operand,                          no_operand,                             opcode_flag_t::none | opcode_flag_t::group },
    { "GRP7",      no_operand,                          no_operand,                             opcode_flag_t::none | opcode_flag_t::group },
    { "LAR",       { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::w }, opcode_flag_t::none },
    { "LSL",       { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::w }, opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "LOADALL_OR_SYSCALL", no_operand,                   no_operand,                             opcode_flag_t::none }, // 0x05 (SYSCALL in 64-bit)
    { "CLTS",      no_operand,                            no_operand,                             opcode_flag_t::none },
    { "SYSRET",    no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVD",      no_operand,                            no_operand,                             opcode_flag_t::none },
    { "WBINVD",    no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "UD2",       no_operand,                            no_operand,                             opcode_flag_t::none }, // 0x0B
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "GRP_NOP",   { opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group }, // 0x0D (Hint NOPs / Prefetch)
    { "FEMMS",     no_operand,                            no_operand,                             opcode_flag_t::none },
    { "3DNOW",     { opaddr_t::P, opsize_t::q }, { opaddr_t::Q, opsize_t::q }, opcode_flag_t::none }, // 0x0F (Requires trailing suffix byte)

    // 0x10 - 0x1F (SSE / SIMD Move instructions)
    { "MOVUPS_OR_MOVUPD",  { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "MOVUPS_OR_MOVUPD",  { opaddr_t::W, opsize_t::ps }, { opaddr_t::V, opsize_t::ps }, opcode_flag_t::none },
    { "MOVLPS_OR_MOVLPD",  { opaddr_t::V, opsize_t::q  }, { opaddr_t::M, opsize_t::q  }, opcode_flag_t::none },
    { "MOVLPS_OR_MOVLPD",  { opaddr_t::M, opsize_t::q  }, { opaddr_t::V, opsize_t::q  }, opcode_flag_t::none },
    { "UNPCKLPS_OR_UNPCKLPD", { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "UNPCKHPS_OR_UNPCKHPD", { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "MOVHPS_OR_MOVHPD",  { opaddr_t::V, opsize_t::q  }, { opaddr_t::M, opsize_t::q  }, opcode_flag_t::none },
    { "MOVHPS_OR_MOVHPD",  { opaddr_t::M, opsize_t::q  }, { opaddr_t::V, opsize_t::q  }, opcode_flag_t::none },
    { "GRP16",     { opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::none }, // 0x18 (Prefetch group)
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    { "NOP",       { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none }, // 0x1F (Multi-byte NOP)

    // 0x20 - 0x2F (Control/Debug Register Moves & SIMD conversions)
    { "MOV",       { opaddr_t::C, opsize_t::y }, { opaddr_t::G, opsize_t::y }, opcode_flag_t::none }, // CR moves
    { "MOV",       { opaddr_t::D, opsize_t::y }, { opaddr_t::G, opsize_t::y }, opcode_flag_t::none }, // DR moves
    { "MOV",       { opaddr_t::G, opsize_t::y }, { opaddr_t::C, opsize_t::y }, opcode_flag_t::none },
    { "MOV",       { opaddr_t::G, opsize_t::y }, { opaddr_t::D, opsize_t::y }, opcode_flag_t::none },
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    { "MOVAPS",    { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none }, // 0x28
    { "MOVAPS",    { opaddr_t::W, opsize_t::ps }, { opaddr_t::V, opsize_t::ps }, opcode_flag_t::none },
    { "CVTPI2PS_OR_CVTTPS2PI", { opaddr_t::V, opsize_t::ps }, { opaddr_t::Q, opsize_t::q }, opcode_flag_t::none },
    { "MOVNTPS_OR_MOVNTPD",    { opaddr_t::M, opsize_t::ps }, { opaddr_t::V, opsize_t::ps }, opcode_flag_t::none },
    { "CVTPS2PI_OR_CVTPI2PD",  { opaddr_t::Q, opsize_t::q  }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "CVTTPS2PI_OR_CVTTPD2PI", { opaddr_t::Q, opsize_t::q  }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "UCOMISS_OR_UCOMISD",    { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "COMISS_OR_COMISD",      { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },

    // 0x30 - 0x3F (System instructions & Multi-byte extensions escapes)
    { "WRMSR",     no_operand,                            no_operand,                             opcode_flag_t::none }, // 0x30
    { "RDTSC",     no_operand,                            no_operand,                             opcode_flag_t::none },
    { "RDMSR",     no_operand,                            no_operand,                             opcode_flag_t::none },
    { "RDPMC",     no_operand,                            no_operand,                             opcode_flag_t::none },
    { "SYSENTER",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "SYSEXIT",   no_operand,                            no_operand,                             opcode_flag_t::none },
    no_opcode,
    { "GETSEC",    no_operand,                            no_operand,                             opcode_flag_t::none },
    { "ESCAPE_3BYTE_1", no_operand,                       no_operand,                             opcode_flag_t::none }, // 0x38 (0x0F 0x38)
    no_opcode,
    { "ESCAPE_3BYTE_2", no_operand,                       no_operand,                             opcode_flag_t::none }, // 0x3A (0x0F 0x3A)
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,

    // 0x40 - 0x4F (CMOVcc operations)
    { "CMOVO",     { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNO",    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVB",     { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNB",    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVZ",     { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNZ",    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVBE",    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNBE",   { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVS",     { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none }, // 0x48
    { "CMOVNS",    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVP",     { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNP",    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVL",     { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNL",    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVLE",    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNLE",   { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },

    // 0x50 - 0x5F (SIMD Floating-Point Math)
    { "MOVMSKPS_OR_MOVMSKPD", { opaddr_t::G, opsize_t::v }, { opaddr_t::V, opsize_t::ps }, opcode_flag_t::none },
    { "SQRTPS_OR_SQRTPD",     { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "RSQRTPS_OR_RSQRTSS",   { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "RCPPS_OR_RCPSS",       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "ANDPS_OR_ANDPD",       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "ANDNPS_OR_ANDNPD",     { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "ORPS_OR_ORPD",         { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "XORPS_OR_XORPD",       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "ADDPS_OR_ADDPD",       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none }, // 0x58
    { "MULPS_OR_MULPD",       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "CVTPS2PD_OR_CVTPD2PS", { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "CVTDQ2PS_OR_CVTPS2DQ", { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "SUBPS_OR_SUBPD",       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "MINPS_OR_MINPD",       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "DIVPS_OR_DIVPD",       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "MAXPS_OR_MAXPD",       { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },

    // 0x60 - 0x6F (MMX / SSE Packed Integer Operations)
    { "PUNPCLBW",  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::d },  opcode_flag_t::none },
    { "PUNPCLWD",  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::d },  opcode_flag_t::none },
    { "PUNPCLDQ",  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::d },  opcode_flag_t::none },
    { "PACKSSWB",  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PCMPGTB",   { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PCMPGTW",   { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PCMPGTD",   { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PACKUSWB",  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PUNPCHBW",  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none }, // 0x68
    { "PUNPCHWD",  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PUNPCHDQ",  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PACKSSDW",  { opaddr_t::P, opsize_t::q },  { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PUNPCHQDQ", { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PUNPCLQDQ", { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "MOVD_OR_MOVQ", { opaddr_t::P, opsize_t::d }, { opaddr_t::Q, opsize_t::d }, opcode_flag_t::none },
    { "MOVDQA_OR_MOVDQU", { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },

    // 0x70 - 0x7F (SIMD Shuffles and Logic shifts)
    { "PSHUFW_OR_PSHUFDB", { opaddr_t::Q, opsize_t::q }, { opaddr_t::N, opsize_t::q }, opcode_flag_t::none },
    { "GRP12",     { opaddr_t::N, opsize_t::q }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group }, // Shift Group (psllw, etc)
    { "GRP13",     { opaddr_t::N, opsize_t::q }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group }, // Shift Group (psraw, etc)
    { "GRP14",     { opaddr_t::N, opsize_t::q }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group }, // Shift Group (psllq, etc)
    { "PCMPEQB",   { opaddr_t::P, opsize_t::q }, { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PCMPEQW",   { opaddr_t::P, opsize_t::q }, { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PCMPEQD",   { opaddr_t::P, opsize_t::q }, { opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "EMMS",      no_operand,                            no_operand,                             opcode_flag_t::none },
    { "VMREAD",    { opaddr_t::E, opsize_t::q }, { opaddr_t::G, opsize_t::q },  opcode_flag_t::none }, // 0x78 (VMX)
    { "VMWRITE",   { opaddr_t::G, opsize_t::q }, { opaddr_t::E, opsize_t::q },  opcode_flag_t::none }, // 0x79 (VMX)
    no_opcode,
    no_opcode,
    { "HADDPD_OR_HADDPS",  { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "HSUBPD_OR_HSUBPS",  { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "MOVD_OR_MOVQ", { opaddr_t::Q, opsize_t::d }, { opaddr_t::P, opsize_t::d }, opcode_flag_t::none },
    { "MOVDQA_OR_MOVDQU", { opaddr_t::W, opsize_t::dq }, { opaddr_t::V, opsize_t::dq }, opcode_flag_t::none },

    // 0x80 - 0x8F (Jcc near branch targets - 16/32-bit offsets)
    { "JO",        { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNO",       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JB",        { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNB",       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JZ",        { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNZ",       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JBE",       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNBE",      { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JS",        { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none }, // 0x88
    { "JNS",       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JP",        { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNP",       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JL",        { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNL",       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JLE",       { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNLE",      { opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },

    // 0x90 - 0x9F (SETcc operations)
    { "SETO",      { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNO",     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETB",      { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNB",     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETZ",      { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNZ",     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETBE",     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNBE",    { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETS",      { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none }, // 0x98
    { "SETNS",     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETP",      { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNP",     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETL",      { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNL",     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETLE",     { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNLE",    { opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },

    // 0xA0 - 0xAF (Segment pointer pushes/pops, CPUID, bit scanning operations)
    { "PUSH",      { opaddr_t::O, opsize_t::w, { register_t::fs } }, no_operand,                             opcode_flag_t::none },
    { "POP",       { opaddr_t::O, opsize_t::w, { register_t::fs } }, no_operand,                             opcode_flag_t::none },
    { "CPUID",     no_operand,                            no_operand,                             opcode_flag_t::none },
    { "BT",        { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "SHLD",      { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none }, // 3rd operand implicit imm byte
    { "SHLD",      { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none }, // 3rd operand implicit CL
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "PUSH",      { opaddr_t::O, opsize_t::w, { register_t::gs } }, no_operand,                             opcode_flag_t::none }, // 0xA8
    { "POP",       { opaddr_t::O, opsize_t::w, { register_t::gs } }, no_operand,                             opcode_flag_t::none },
    { "RSM",       no_operand,                            no_operand,                             opcode_flag_t::none },
    { "BTS",       { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "SHRD",      { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "SHRD",      { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "GRP15",     no_operand, no_operand,                             opcode_flag_t::none }, // FXSAVE, FXRSTOR, LFENCE...
    { "IMUL",      { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },

    // 0xB0 - 0xBF (CMPXCHG, Data conversions, Bit Tests)
    { "CMPXCHG",   { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "CMPXCHG",   { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "LSS",       { opaddr_t::G, opsize_t::v }, { opaddr_t::M, opsize_t::p }, opcode_flag_t::none },
    { "BTR",       { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "LFS",       { opaddr_t::G, opsize_t::v }, { opaddr_t::M, opsize_t::p }, opcode_flag_t::none },
    { "LGS",       { opaddr_t::G, opsize_t::v }, { opaddr_t::M, opsize_t::p }, opcode_flag_t::none },
    { "MOVZBL",    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::none },
    { "MOVZWL",    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::w }, opcode_flag_t::none },
    no_opcode,
    { "UD1",       no_operand,                            no_operand,                             opcode_flag_t::none },
    { "GRP8",      no_operand,                            no_operand,                           opcode_flag_t::lockable | opcode_flag_t::group }, // Bit group extensions
    { "BTC",       { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable | opcode_flag_t::group },
    { "BSF",       { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "BSR",       { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "MOVSBL",    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::b }, opcode_flag_t::none },
    { "MOVSWL",    { opaddr_t::G, opsize_t::v }, { opaddr_t::E, opsize_t::w }, opcode_flag_t::none },

    // 0xC0 - 0xCF (XADD, SIMD permutations, and multi-byte additions)
    { "XADD",      { opaddr_t::E, opsize_t::b }, { opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "XADD",      { opaddr_t::E, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "CMPPS_OR_CMPPD",    { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "MOVNTI",    { opaddr_t::M, opsize_t::v }, { opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "PINSRW",    { opaddr_t::P, opsize_t::q }, { opaddr_t::E, opsize_t::w }, opcode_flag_t::none },
    { "PEXTRW",    { opaddr_t::G, opsize_t::v }, { opaddr_t::N, opsize_t::q }, opcode_flag_t::none },
    { "SHUFPS_OR_SHUFPD",  { opaddr_t::V, opsize_t::ps }, { opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "GRP9",      no_operand, no_operand,                             opcode_flag_t::lockable | opcode_flag_t::group }, // 0xC7 (CMPXCHG8B/16B)
    { "BSWAP",     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rax } }, no_operand,                             opcode_flag_t::none }, // 0xC8
    { "BSWAP",     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rcx } }, no_operand,                             opcode_flag_t::none },
    { "BSWAP",     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdx } }, no_operand,                             opcode_flag_t::none },
    { "BSWAP",     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbx } }, no_operand,                             opcode_flag_t::none },
    { "BSWAP",     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsp } }, no_operand,                             opcode_flag_t::none },
    { "BSWAP",     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rbp } }, no_operand,                             opcode_flag_t::none },
    { "BSWAP",     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rsi } }, no_operand,                             opcode_flag_t::none },
    { "BSWAP",     { opaddr_t::A, opsize_t::v, { extended_register_encoding_t::rdi } }, no_operand,                             opcode_flag_t::none },

    // 0xD0 - 0xDF (SSE / MMX Packed Math Continued)
    { "ADDSUBPD_OR_ADDSUBPS", { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PAND",      { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PANDN",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "POR",       { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PXOR",      { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PADDB",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PADDW",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PADDD",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMAXUB",    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none }, // 0xD8
    { "PMINUB",    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMULHUW",   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMULHW",    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "CVTTPD2DQ_OR_CVTPI2PD", { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PINSRD_OR_PINSRQ",      { opaddr_t::V, opsize_t::dq }, { opaddr_t::E, opsize_t::d  }, opcode_flag_t::none },
    { "PEXTRD_OR_PEXTRQ",      { opaddr_t::E, opsize_t::d  }, { opaddr_t::V, opsize_t::dq }, opcode_flag_t::none },
    { "PMULUDQ",   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },

    // 0xE0 - 0xEF
    { "PAVGB",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSRAW",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSRAD",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMINSW",    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMAXSW",    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMADDWD",   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSADBW",    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "MASKMOVQ_OR_MASKMOVDQU", { opaddr_t::P, opsize_t::q  }, { opaddr_t::N, opsize_t::q  }, opcode_flag_t::none },
    { "PSUBB",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none }, // 0xE8
    { "PSUBW",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSUBD",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PEXTRW",    { opaddr_t::G, opsize_t::v  }, { opaddr_t::V, opsize_t::dq }, opcode_flag_t::none },
    { "PMULLW",    { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMULILDQ",  { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "MOVDQ2Q",   { opaddr_t::P, opsize_t::q  }, { opaddr_t::W, opsize_t::q  }, opcode_flag_t::none },
    { "MOVQ2DQ",   { opaddr_t::V, opsize_t::dq }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },

    // 0xF0 - 0xFF
    no_opcode,
    { "PSLLW",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSLLD",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSLLQ",     { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMULUDQ",   { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PMADDUBW",  { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PSUBUSB",   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSUBUSW",   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMINSD",    { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none }, // 0xF8
    { "PMAXSD",    { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PEXTRW",    { opaddr_t::E, opsize_t::w  }, { opaddr_t::V, opsize_t::dq }, opcode_flag_t::none },
    { "PADDUSB",   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PADDUSW",   { opaddr_t::P, opsize_t::q  }, { opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMAXUD",    { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PMINUD",    { opaddr_t::V, opsize_t::dq }, { opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    no_opcode,
};

// Group 1: Immediate Extensions (Opcodes: 80, 81, 82, 83)
// Note: Operand 2 size is dynamically resolved ('v' or 'b') depending on the primary opcode
const opcode_t table_group_1[8] = {
    { "ADD", { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { "OR",  { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { "ADC", { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { "SBB", { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { "AND", { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { "SUB", { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { "XOR", { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::lockable },
    { "TXT", { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::none }, opcode_flag_t::none } // CMP semantics
};

// Group 2: Shift & Rotate Extensions (Opcodes: C0, C1, D0, D1, D2, D3)
const opcode_t table_group_2[8] = {
    { "ROL", { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "ROR", { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "RCL", { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "RCR", { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "SHL", { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "SHR", { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "SAL", { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "SAR", { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none }
};

// Group 3: Unary Operations / Multiply / Divide (Opcodes: F6, F7)
const opcode_t table_group_3[8] = {
    { "TEST",    { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::v },    opcode_flag_t::none },
    no_opcode,
    { "NOT",     { opaddr_t::E, opsize_t::v }, no_operand,                            opcode_flag_t::lockable },
    { "NEG",     { opaddr_t::E, opsize_t::v }, no_operand,                            opcode_flag_t::lockable },
    { "MUL",     { opaddr_t::E, opsize_t::v }, no_operand,                            opcode_flag_t::none },
    { "IMUL",    { opaddr_t::E, opsize_t::v }, no_operand,                            opcode_flag_t::none },
    { "DIV",     { opaddr_t::E, opsize_t::v }, no_operand,                            opcode_flag_t::none },
    { "IDIV",    { opaddr_t::E, opsize_t::v }, no_operand,                            opcode_flag_t::none }
};

// Group 4: Increment / Decrement 8-bit (Opcode: FE)
const opcode_t table_group_4[8] = {
    { "INC",     { opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::lockable },
    { "DEC",     { opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::lockable },
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
};

// Group 5: Near & Far Indirect Jumps / Calls / Pushes (Opcode: FF)
const opcode_t table_group_5[8] = {
    { "INC",     { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::lockable },
    { "DEC",     { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::lockable },
    { "CALL",    { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none }, // Near
    { "CALL",    { opaddr_t::E, opsize_t::p }, no_operand, opcode_flag_t::none }, // Far
    { "JMP",     { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none }, // Near
    { "JMP",     { opaddr_t::E, opsize_t::p }, no_operand, opcode_flag_t::none }, // Far
    { "PUSH",    { opaddr_t::E, opsize_t::v }, no_operand, opcode_flag_t::none },
    no_opcode,
};

// Group 6: System Tables Architecture (Opcode: 0F 00)
const opcode_t table_group_6[8] = {
    { "SLDT",    { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none },
    { "STR",     { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none },
    { "LLDT",    { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none },
    { "LTR",     { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none },
    { "VERR",    { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none },
    { "VERW",    { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none },
    no_opcode,
    no_opcode,
};

// Group 7: Descriptor Tables & System States (Opcode: 0F 01)
const opcode_t table_group_7[8] = {
    { "SGDT",    { opaddr_t::M, opsize_t::p }, no_operand, opcode_flag_t::none },
    { "SIDT",    { opaddr_t::M, opsize_t::p }, no_operand, opcode_flag_t::none },
    { "LGDT",    { opaddr_t::M, opsize_t::p }, no_operand, opcode_flag_t::none },
    { "LIDT",    { opaddr_t::M, opsize_t::p }, no_operand, opcode_flag_t::none },
    { "SMSW",    { opaddr_t::E, opsize_t::w }, no_operand, opcode_flag_t::none }, // Maps inside MSW slot
    no_opcode,
    { "INVLPG",  { opaddr_t::M, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "GRP7_EXT",no_operand,                            no_operand, opcode_flag_t::none } // Escapes to direct register matchers (SWAPGS, etc)
};

// Group 8: Bit Tests with Immediates (Opcode: 0F BA)
const opcode_t table_group_8[8] = {
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    { "BT",      { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "BTC",     { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable },
    { "BTR",     { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable },
    { "BTS",     { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable }
};

// Group 9: Compare and Exchange (Opcode: 0F C7)
const opcode_t table_group_9[8] = {
    no_opcode,
    { "CMPXCHG8B", { opaddr_t::M, opsize_t::q }, no_operand, opcode_flag_t::lockable }, // Resolves to 16B if REX.W=1
    no_opcode,
    no_opcode,
    no_opcode,
    no_opcode,
    { "VMPTRLD",   { opaddr_t::M, opsize_t::q }, no_operand, opcode_flag_t::none },
    { "VMPTRST",   { opaddr_t::M, opsize_t::q }, no_operand, opcode_flag_t::none }
};

// Group 11: Immediate Move Block (Opcodes: C6, C7)
const opcode_t table_group_11[8] = {
    { "MOV",     { opaddr_t::E, opsize_t::v }, { opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
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
    { "FXSAVE",     { opaddr_t::M, opsize_t::none }, no_operand, opcode_flag_t::none },
    { "FXRSTOR",    { opaddr_t::M, opsize_t::none }, no_operand, opcode_flag_t::none },
    { "LDMXCSR",    { opaddr_t::M, opsize_t::d    }, no_operand, opcode_flag_t::none },
    { "STMXCSR",    { opaddr_t::M, opsize_t::d    }, no_operand, opcode_flag_t::none },
    { "XSAVE",      { opaddr_t::M, opsize_t::none }, no_operand, opcode_flag_t::none }, // LFENCE space if Mod == 11
    { "XRSTOR",     { opaddr_t::M, opsize_t::none }, no_operand, opcode_flag_t::none }, // MFENCE space if Mod == 11
    { "CLFLUSH",    { opaddr_t::M, opsize_t::b    }, no_operand, opcode_flag_t::none }, // SFENCE space if Mod == 11
    { "CLFLUSHOPT", { opaddr_t::M, opsize_t::b    }, no_operand, opcode_flag_t::none }
};

}
