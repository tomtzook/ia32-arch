
#include "x86/types.h"
#include "x86/opcode.h"

namespace x86::opcode::def {

constexpr operand_t no_operand = { nullptr, opaddr_t::A, opsize_t::a, false };

// Opcode maps are in Intel SDM Vol 2 Appendix A.3
// also found here https://www.sandpile.org

const opcode_t table_primary[256] = {
    // 0x00 - 0x0F
    { "ADD",  { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "ADD",  { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "ADD",  { nullptr, opaddr_t::G, opsize_t::b }, { nullptr, opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "ADD",  { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "ADD",  { "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "ADD",  { "rAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "PUSH", { "ES",    opaddr_t::S, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "POP",  { "ES",    opaddr_t::S, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "OR",   { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "OR",   { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "OR",   { nullptr, opaddr_t::G, opsize_t::b }, { nullptr, opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "OR",   { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "OR",   { "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "OR",   { "rAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "PUSH", { "CS",    opaddr_t::S, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "ESCAPE_2BYTE", no_operand,                    no_operand,                             opcode_flag_t::none },

    // 0x10 - 0x1F
    { "ADC",  { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "ADC",  { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "ADC",  { nullptr, opaddr_t::G, opsize_t::b }, { nullptr, opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "ADC",  { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "ADC",  { "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "ADC",  { "rAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "PUSH", { "SS",    opaddr_t::S, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "POP",  { "SS",    opaddr_t::S, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "SBB",  { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "SBB",  { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "SBB",  { nullptr, opaddr_t::G, opsize_t::b }, { nullptr, opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "SBB",  { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "SBB",  { "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "SBB",  { "rAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "PUSH", { "DS",    opaddr_t::S, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "POP",  { "DS",    opaddr_t::S, opsize_t::w }, no_operand,                             opcode_flag_t::none },

    // 0x20 - 0x2F
    { "AND",  { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "AND",  { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "AND",  { nullptr, opaddr_t::G, opsize_t::b }, { nullptr, opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "AND",  { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "AND",  { "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "AND",  { "rAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "SEG_ES",no_operand,                           no_operand,                             opcode_flag_t::none },
    { "DAA",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "SUB",  { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "SUB",  { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "SUB",  { nullptr, opaddr_t::G, opsize_t::b }, { nullptr, opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "SUB",  { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "SUB",  { "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "SUB",  { "rAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "REG_CS_OR_BR_NOT_TAKEN", no_operand,          no_operand,                             opcode_flag_t::none },
    { "DAS",  no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0x30 - 0x3F
    { "XOR",  { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "XOR",  { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "XOR",  { nullptr, opaddr_t::G, opsize_t::b }, { nullptr, opaddr_t::E, opsize_t::b }, opcode_flag_t::lockable },
    { "XOR",  { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::lockable },
    { "XOR",  { "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "XOR",  { "rAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "SEG_SS",no_operand,                           no_operand,                             opcode_flag_t::none },
    { "AAA",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "CMP",  { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::none },
    { "CMP",  { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "CMP",  { nullptr, opaddr_t::G, opsize_t::b }, { nullptr, opaddr_t::E, opsize_t::b }, opcode_flag_t::none },
    { "CMP",  { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMP",  { "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "CMP",  { "rAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
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
    { "PUSH", { "rAX",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "PUSH", { "rCX",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "PUSH", { "rDX",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "PUSH", { "rBX",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "PUSH", { "rSP",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "PUSH", { "rBP",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "PUSH", { "rSI",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "PUSH", { "rDI",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "POP",  { "rAX",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "POP",  { "rCX",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "POP",  { "rDX",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "POP",  { "rBX",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "POP",  { "rSP",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "POP",  { "rBP",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "POP",  { "rSI",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },
    { "POP",  { "rDI",   opaddr_t::A, opsize_t::v }, no_operand, opcode_flag_t::none },

    // 0x60 - 0x6F
    { "PUSHA", no_operand,                           no_operand,                             opcode_flag_t::none },
    { "POPA",  no_operand,                           no_operand,                             opcode_flag_t::none },
    { "BOUND_OR_EVEX", no_operand,                   no_operand,                             opcode_flag_t::none },
    { "MOVSXD",{ nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::d }, opcode_flag_t::none },
    { "SEG_FS",no_operand,                           no_operand,                             opcode_flag_t::none },
    { "SEG_GS",no_operand,                           no_operand,                             opcode_flag_t::none },
    { "PREFIX_OPERAND_SIZE", no_operand,              no_operand,                             opcode_flag_t::none },
    { "PREFIX_ADDRESS_SIZE", no_operand,              no_operand,                             opcode_flag_t::none },
    { "PUSH", { nullptr, opaddr_t::I, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "IMUL", { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "PUSH", { nullptr, opaddr_t::I, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "IMUL", { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "INSB", { nullptr, opaddr_t::Y, opsize_t::b }, { "DX",    opaddr_t::A, opsize_t::w }, opcode_flag_t::none },
    { "INSW", { nullptr, opaddr_t::Y, opsize_t::v }, { "DX",    opaddr_t::A, opsize_t::w }, opcode_flag_t::none },
    { "OUTSB",{ "DX",    opaddr_t::A, opsize_t::w }, { nullptr, opaddr_t::X, opsize_t::b }, opcode_flag_t::none },
    { "OUTSW",{ "DX",    opaddr_t::A, opsize_t::w }, { nullptr, opaddr_t::X, opsize_t::v }, opcode_flag_t::none },

    // 0x70 - 0x7F
    { "JO",   { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNO",  { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JB",   { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNB",  { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JZ",   { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNZ",  { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JBE",  { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNBE", { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JS",   { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNS",  { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JP",   { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNP",  { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JL",   { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNL",  { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JLE",  { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "JNLE", { nullptr, opaddr_t::J, opsize_t::b }, no_operand, opcode_flag_t::none },

    // 0x80 - 0x8F
    { "GRP1", { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable | opcode_flag_t::group },
    { "GRP1", { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::z }, opcode_flag_t::lockable | opcode_flag_t::group },
    { "GRP1", { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable | opcode_flag_t::group },
    { "GRP1", { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable | opcode_flag_t::group },
    { "TEST", { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::none },
    { "TEST", { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "XCHG", { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "XCHG", { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "MOV",  { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { nullptr, opaddr_t::G, opsize_t::b }, { nullptr, opaddr_t::E, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::S, opsize_t::w }, opcode_flag_t::none },
    { "LEA",  { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::M, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { nullptr, opaddr_t::S, opsize_t::w }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "GRP1A",{ nullptr, opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::none },

    // 0x90 - 0x9F
    { "NOP",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "XCHG", { "rCX",   opaddr_t::A, opsize_t::v }, { "rAX",   opaddr_t::A, opsize_t::v }, opcode_flag_t::none },
    { "XCHG", { "rDX",   opaddr_t::A, opsize_t::v }, { "rAX",   opaddr_t::A, opsize_t::v }, opcode_flag_t::none },
    { "XCHG", { "rBX",   opaddr_t::A, opsize_t::v }, { "rAX",   opaddr_t::A, opsize_t::v }, opcode_flag_t::none },
    { "XCHG", { "rSP",   opaddr_t::A, opsize_t::v }, { "rAX",   opaddr_t::A, opsize_t::v }, opcode_flag_t::none },
    { "XCHG", { "rBP",   opaddr_t::A, opsize_t::v }, { "rAX",   opaddr_t::A, opsize_t::v }, opcode_flag_t::none },
    { "XCHG", { "rSI",   opaddr_t::A, opsize_t::v }, { "rAX",   opaddr_t::A, opsize_t::v }, opcode_flag_t::none },
    { "XCHG", { "rDI",   opaddr_t::A, opsize_t::v }, { "rAX",   opaddr_t::A, opsize_t::v }, opcode_flag_t::none },
    { "CBW",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "CWD",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "CALLF",no_operand,                            no_operand,                             opcode_flag_t::none },
    { "WAIT", no_operand,                            no_operand,                             opcode_flag_t::none },
    { "PUSHF",{ nullptr, opaddr_t::F, opsize_t::v }, no_operand,                             opcode_flag_t::none },
    { "POPF", { nullptr, opaddr_t::F, opsize_t::v }, no_operand,                             opcode_flag_t::none },
    { "SAHF", no_operand,                            no_operand,                             opcode_flag_t::none },
    { "LAHF", no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0xA0 - 0xAF
    { "MOV",  { "AL",    opaddr_t::A, opsize_t::b }, { "Ob",    opaddr_t::A, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { "rAX",   opaddr_t::A, opsize_t::v }, { "Ov",    opaddr_t::A, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { "Ob",    opaddr_t::A, opsize_t::b }, { "AL",    opaddr_t::A, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { "Ov",    opaddr_t::A, opsize_t::v }, { "rAX",   opaddr_t::A, opsize_t::v }, opcode_flag_t::none },
    { "MOVSB",{ nullptr, opaddr_t::Y, opsize_t::b }, { nullptr, opaddr_t::X, opsize_t::b }, opcode_flag_t::none },
    { "MOVSW",{ nullptr, opaddr_t::Y, opsize_t::v }, { nullptr, opaddr_t::X, opsize_t::v }, opcode_flag_t::none },
    { "CMPSB",{ nullptr, opaddr_t::X, opsize_t::b }, { nullptr, opaddr_t::Y, opsize_t::b }, opcode_flag_t::none },
    { "CMPSW",{ nullptr, opaddr_t::X, opsize_t::v }, { nullptr, opaddr_t::Y, opsize_t::v }, opcode_flag_t::none },
    { "TEST", { "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "TEST", { "rAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::z }, opcode_flag_t::none },
    { "STOSB",{ nullptr, opaddr_t::Y, opsize_t::b }, { "AL",    opaddr_t::A, opsize_t::b }, opcode_flag_t::none },
    { "STOSW",{ nullptr, opaddr_t::Y, opsize_t::v }, { "rAX",   opaddr_t::A, opsize_t::v }, opcode_flag_t::none },
    { "LODSB",{ "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::X, opsize_t::b }, opcode_flag_t::none },
    { "LODSW",{ "rAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::X, opsize_t::v }, opcode_flag_t::none },
    { "SCASB",{ "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::Y, opsize_t::b }, opcode_flag_t::none },
    { "SCASW",{ "rAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::Y, opsize_t::v }, opcode_flag_t::none },

    // 0xB0 - 0xBF
    { "MOV",  { "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { "CL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { "DL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { "BL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { "AH",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { "CH",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { "DH",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { "BH",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "MOV",  { "rAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { "rCX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { "rDX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { "rBX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { "rSP",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { "rBP",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { "rSI",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::v }, opcode_flag_t::none },
    { "MOV",  { "rDI",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::v }, opcode_flag_t::none },

    // 0xC0 - 0xCF
    { "GRP2", { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none | opcode_flag_t::group },
    { "GRP2", { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none | opcode_flag_t::group },
    { "RET",  { nullptr, opaddr_t::I, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "RET",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "ESCAPE_VEX_3BYTE", no_operand,                no_operand,                             opcode_flag_t::none }, // 0xC4
    { "ESCAPE_VEX_2BYTE", no_operand,                no_operand,                             opcode_flag_t::none }, // 0xC5
    { "GRP11",{ nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none | opcode_flag_t::group },
    { "GRP11",{ nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::z }, opcode_flag_t::none | opcode_flag_t::group },
    { "ENTER",{ nullptr, opaddr_t::I, opsize_t::w }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none | opcode_flag_t::group },
    { "LEAVE",no_operand,                            no_operand,                             opcode_flag_t::none },
    { "RETF", { nullptr, opaddr_t::I, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "RETF", no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INT3", no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INT",  { nullptr, opaddr_t::I, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "INTO", no_operand,                            no_operand,                             opcode_flag_t::none },
    { "IRET", no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0xD0 - 0xD7
    { "GRP2", { nullptr, opaddr_t::E, opsize_t::b }, { "1",     opaddr_t::A, opsize_t::b }, opcode_flag_t::none | opcode_flag_t::group },
    { "GRP2", { nullptr, opaddr_t::E, opsize_t::v }, { "1",     opaddr_t::A, opsize_t::b }, opcode_flag_t::none | opcode_flag_t::group },
    { "GRP2", { nullptr, opaddr_t::E, opsize_t::b }, { "CL",    opaddr_t::A, opsize_t::b }, opcode_flag_t::none | opcode_flag_t::group },
    { "GRP2", { nullptr, opaddr_t::E, opsize_t::v }, { "CL",    opaddr_t::A, opsize_t::b }, opcode_flag_t::none | opcode_flag_t::group },
    { "AAM",  { nullptr, opaddr_t::I, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "AAD",  { nullptr, opaddr_t::I, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SALC", no_operand,                            no_operand,                             opcode_flag_t::none },
    { "XLAT", no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0xD8 - 0xDF (FPU Escapes)
    { "ESCAPE_FPU", { nullptr, opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { nullptr, opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { nullptr, opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { nullptr, opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { nullptr, opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { nullptr, opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { nullptr, opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },
    { "ESCAPE_FPU", { nullptr, opaddr_t::E, opsize_t::b }, no_operand, opcode_flag_t::none },

    // 0xE0 - 0xEF
    { "LOOPNE", { nullptr, opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "LOOPE",  { nullptr, opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "LOOP",   { nullptr, opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "jCXZ",   { nullptr, opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "IN",     { "AL",    opaddr_t::A, opsize_t::b }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "IN",     { "eAX",   opaddr_t::A, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::none },
    { "OUT",    { nullptr, opaddr_t::I, opsize_t::b }, { "AL",    opaddr_t::A, opsize_t::b }, opcode_flag_t::none },
    { "OUT",    { nullptr, opaddr_t::I, opsize_t::b }, { "eAX",   opaddr_t::A, opsize_t::v }, opcode_flag_t::none },
    { "CALL",   { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JMP",    { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JMPF",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "JMP",    { nullptr, opaddr_t::J, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "IN",     { "AL",    opaddr_t::A, opsize_t::b }, { "DX",    opaddr_t::A, opsize_t::w }, opcode_flag_t::none },
    { "IN",     { "eAX",   opaddr_t::A, opsize_t::v }, { "DX",    opaddr_t::A, opsize_t::w }, opcode_flag_t::none },
    { "OUT",    { "DX",    opaddr_t::A, opsize_t::w }, { "AL",    opaddr_t::A, opsize_t::b }, opcode_flag_t::none },
    { "OUT",    { "DX",    opaddr_t::A, opsize_t::w }, { "eAX",   opaddr_t::A, opsize_t::v }, opcode_flag_t::none },

    // 0xF0 - 0xFF
    { "PREFIX_LOCK", no_operand,                     no_operand,                             opcode_flag_t::none },
    { "ICEBP",  no_operand,                          no_operand,                             opcode_flag_t::none },
    { "PREFIX_REPNE", no_operand,                    no_operand,                             opcode_flag_t::none },
    { "PREFIX_REP", no_operand,                      no_operand,                             opcode_flag_t::none },
    { "HLT",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "CMC",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "GRP3",   { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group },
    { "GRP3",   { nullptr, opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group },
    { "CLC",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "STC",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "CLI",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "STI",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "CLD",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "STD",    no_operand,                          no_operand,                             opcode_flag_t::none },
    { "GRP4",   { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group },
    { "GRP5",   { nullptr, opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group }
};

const opcode_t table_extended_2byte[256] = {
    // 0x00 - 0x0F
    { "GRP6",      { nullptr, opaddr_t::E, opsize_t::w }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group },
    { "GRP7",      { nullptr, opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group },
    { "LAR",       { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::w }, opcode_flag_t::none },
    { "LSL",       { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::w }, opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "LOADALL_OR_SYSCALL", no_operand,                   no_operand,                             opcode_flag_t::none }, // 0x05 (SYSCALL in 64-bit)
    { "CLTS",      no_operand,                            no_operand,                             opcode_flag_t::none },
    { "SYSRET",    no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVD",      no_operand,                            no_operand,                             opcode_flag_t::none },
    { "WBINVD",    no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "UD2",       no_operand,                            no_operand,                             opcode_flag_t::none }, // 0x0B
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "GRP_NOP",   { nullptr, opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group }, // 0x0D (Hint NOPs / Prefetch)
    { "FEMMS",     no_operand,                            no_operand,                             opcode_flag_t::none },
    { "3DNOW",     { nullptr, opaddr_t::P, opsize_t::q }, { nullptr, opaddr_t::Q, opsize_t::q }, opcode_flag_t::none }, // 0x0F (Requires trailing suffix byte)

    // 0x10 - 0x1F (SSE / SIMD Move instructions)
    { "MOVUPS_OR_MOVUPD",  { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "MOVUPS_OR_MOVUPD",  { nullptr, opaddr_t::W, opsize_t::ps }, { nullptr, opaddr_t::V, opsize_t::ps }, opcode_flag_t::none },
    { "MOVLPS_OR_MOVLPD",  { nullptr, opaddr_t::V, opsize_t::q  }, { nullptr, opaddr_t::M, opsize_t::q  }, opcode_flag_t::none },
    { "MOVLPS_OR_MOVLPD",  { nullptr, opaddr_t::M, opsize_t::q  }, { nullptr, opaddr_t::V, opsize_t::q  }, opcode_flag_t::none },
    { "UNPCKLPS_OR_UNPCKLPD", { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "UNPCKHPS_OR_UNPCKHPD", { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "MOVHPS_OR_MOVHPD",  { nullptr, opaddr_t::V, opsize_t::q  }, { nullptr, opaddr_t::M, opsize_t::q  }, opcode_flag_t::none },
    { "MOVHPS_OR_MOVHPD",  { nullptr, opaddr_t::M, opsize_t::q  }, { nullptr, opaddr_t::V, opsize_t::q  }, opcode_flag_t::none },
    { "GRP16",     { nullptr, opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::none }, // 0x18 (Prefetch group)
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none | opcode_flag_t::group },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "NOP",       { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::none }, // 0x1F (Multi-byte NOP)

    // 0x20 - 0x2F (Control/Debug Register Moves & SIMD conversions)
    { "MOV",       { nullptr, opaddr_t::C, opsize_t::y }, { nullptr, opaddr_t::G, opsize_t::y }, opcode_flag_t::none }, // CR moves
    { "MOV",       { nullptr, opaddr_t::D, opsize_t::y }, { nullptr, opaddr_t::G, opsize_t::y }, opcode_flag_t::none }, // DR moves
    { "MOV",       { nullptr, opaddr_t::G, opsize_t::y }, { nullptr, opaddr_t::C, opsize_t::y }, opcode_flag_t::none },
    { "MOV",       { nullptr, opaddr_t::G, opsize_t::y }, { nullptr, opaddr_t::D, opsize_t::y }, opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "MOVAPS",    { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none }, // 0x28
    { "MOVAPS",    { nullptr, opaddr_t::W, opsize_t::ps }, { nullptr, opaddr_t::V, opsize_t::ps }, opcode_flag_t::none },
    { "CVTPI2PS_OR_CVTTPS2PI", { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::Q, opsize_t::q }, opcode_flag_t::none },
    { "MOVNTPS_OR_MOVNTPD",    { nullptr, opaddr_t::M, opsize_t::ps }, { nullptr, opaddr_t::V, opsize_t::ps }, opcode_flag_t::none },
    { "CVTPS2PI_OR_CVTPI2PD",  { nullptr, opaddr_t::Q, opsize_t::q  }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "CVTTPS2PI_OR_CVTTPD2PI", { nullptr, opaddr_t::Q, opsize_t::q  }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "UCOMISS_OR_UCOMISD",    { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "COMISS_OR_COMISD",      { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },

    // 0x30 - 0x3F (System instructions & Multi-byte extensions escapes)
    { "WRMSR",     no_operand,                            no_operand,                             opcode_flag_t::none }, // 0x30
    { "RDTSC",     no_operand,                            no_operand,                             opcode_flag_t::none },
    { "RDMSR",     no_operand,                            no_operand,                             opcode_flag_t::none },
    { "RDPMC",     no_operand,                            no_operand,                             opcode_flag_t::none },
    { "SYSENTER",  no_operand,                            no_operand,                             opcode_flag_t::none },
    { "SYSEXIT",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "GETSEC",    no_operand,                            no_operand,                             opcode_flag_t::none },
    { "ESCAPE_3BYTE_1", no_operand,                       no_operand,                             opcode_flag_t::none }, // 0x38 (0x0F 0x38)
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "ESCAPE_3BYTE_2", no_operand,                       no_operand,                             opcode_flag_t::none }, // 0x3A (0x0F 0x3A)
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },

    // 0x40 - 0x4F (CMOVcc operations)
    { "CMOVO",     { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNO",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVB",     { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNB",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVZ",     { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNZ",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVBE",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNBE",   { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVS",     { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none }, // 0x48
    { "CMOVNS",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVP",     { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNP",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVL",     { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNL",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVLE",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "CMOVNLE",   { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },

    // 0x50 - 0x5F (SIMD Floating-Point Math)
    { "MOVMSKPS_OR_MOVMSKPD", { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::V, opsize_t::ps }, opcode_flag_t::none },
    { "SQRTPS_OR_SQRTPD",     { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "RSQRTPS_OR_RSQRTSS",   { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "RCPPS_OR_RCPSS",       { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "ANDPS_OR_ANDPD",       { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "ANDNPS_OR_ANDNPD",     { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "ORPS_OR_ORPD",         { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "XORPS_OR_XORPD",       { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "ADDPS_OR_ADDPD",       { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none }, // 0x58
    { "MULPS_OR_MULPD",       { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "CVTPS2PD_OR_CVTPD2PS", { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "CVTDQ2PS_OR_CVTPS2DQ", { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "SUBPS_OR_SUBPD",       { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "MINPS_OR_MINPD",       { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "DIVPS_OR_DIVPD",       { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "MAXPS_OR_MAXPD",       { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },

    // 0x60 - 0x6F (MMX / SSE Packed Integer Operations)
    { "PUNPCLBW",  { nullptr, opaddr_t::P, opsize_t::q },  { nullptr, opaddr_t::Q, opsize_t::d },  opcode_flag_t::none },
    { "PUNPCLWD",  { nullptr, opaddr_t::P, opsize_t::q },  { nullptr, opaddr_t::Q, opsize_t::d },  opcode_flag_t::none },
    { "PUNPCLDQ",  { nullptr, opaddr_t::P, opsize_t::q },  { nullptr, opaddr_t::Q, opsize_t::d },  opcode_flag_t::none },
    { "PACKSSWB",  { nullptr, opaddr_t::P, opsize_t::q },  { nullptr, opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PCMPGTB",   { nullptr, opaddr_t::P, opsize_t::q },  { nullptr, opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PCMPGTW",   { nullptr, opaddr_t::P, opsize_t::q },  { nullptr, opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PCMPGTD",   { nullptr, opaddr_t::P, opsize_t::q },  { nullptr, opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PACKUSWB",  { nullptr, opaddr_t::P, opsize_t::q },  { nullptr, opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PUNPCHBW",  { nullptr, opaddr_t::P, opsize_t::q },  { nullptr, opaddr_t::Q, opsize_t::q },  opcode_flag_t::none }, // 0x68
    { "PUNPCHWD",  { nullptr, opaddr_t::P, opsize_t::q },  { nullptr, opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PUNPCHDQ",  { nullptr, opaddr_t::P, opsize_t::q },  { nullptr, opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PACKSSDW",  { nullptr, opaddr_t::P, opsize_t::q },  { nullptr, opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PUNPCHQDQ", { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PUNPCLQDQ", { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "MOVD_OR_MOVQ", { nullptr, opaddr_t::P, opsize_t::d }, { nullptr, opaddr_t::Q, opsize_t::d }, opcode_flag_t::none },
    { "MOVDQA_OR_MOVDQU", { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },

    // 0x70 - 0x7F (SIMD Shuffles and Logic shifts)
    { "PSHUFW_OR_PSHUFDB", { nullptr, opaddr_t::Q, opsize_t::q }, { nullptr, opaddr_t::N, opsize_t::q }, opcode_flag_t::none },
    { "GRP12",     { nullptr, opaddr_t::N, opsize_t::q }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group }, // Shift Group (psllw, etc)
    { "GRP13",     { nullptr, opaddr_t::N, opsize_t::q }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group }, // Shift Group (psraw, etc)
    { "GRP14",     { nullptr, opaddr_t::N, opsize_t::q }, no_operand,                             opcode_flag_t::none | opcode_flag_t::group }, // Shift Group (psllq, etc)
    { "PCMPEQB",   { nullptr, opaddr_t::P, opsize_t::q }, { nullptr, opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PCMPEQW",   { nullptr, opaddr_t::P, opsize_t::q }, { nullptr, opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "PCMPEQD",   { nullptr, opaddr_t::P, opsize_t::q }, { nullptr, opaddr_t::Q, opsize_t::q },  opcode_flag_t::none },
    { "EMMS",      no_operand,                            no_operand,                             opcode_flag_t::none },
    { "VMREAD",    { nullptr, opaddr_t::E, opsize_t::q }, { nullptr, opaddr_t::G, opsize_t::q },  opcode_flag_t::none }, // 0x78 (VMX)
    { "VMWRITE",   { nullptr, opaddr_t::G, opsize_t::q }, { nullptr, opaddr_t::E, opsize_t::q },  opcode_flag_t::none }, // 0x79 (VMX)
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "HADDPD_OR_HADDPS",  { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "HSUBPD_OR_HSUBPS",  { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "MOVD_OR_MOVQ", { nullptr, opaddr_t::Q, opsize_t::d }, { nullptr, opaddr_t::P, opsize_t::d }, opcode_flag_t::none },
    { "MOVDQA_OR_MOVDQU", { nullptr, opaddr_t::W, opsize_t::dq }, { nullptr, opaddr_t::V, opsize_t::dq }, opcode_flag_t::none },

    // 0x80 - 0x8F (Jcc near branch targets - 16/32-bit offsets)
    { "JO",        { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNO",       { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JB",        { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNB",       { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JZ",        { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNZ",       { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JBE",       { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNBE",      { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JS",        { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none }, // 0x88
    { "JNS",       { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JP",        { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNP",       { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JL",        { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNL",       { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JLE",       { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },
    { "JNLE",      { nullptr, opaddr_t::J, opsize_t::z }, no_operand,                             opcode_flag_t::none },

    // 0x90 - 0x9F (SETcc operations)
    { "SETO",      { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNO",     { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETB",      { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNB",     { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETZ",      { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNZ",     { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETBE",     { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNBE",    { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETS",      { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none }, // 0x98
    { "SETNS",     { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETP",      { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNP",     { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETL",      { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNL",     { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETLE",     { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },
    { "SETNLE",    { nullptr, opaddr_t::E, opsize_t::b }, no_operand,                             opcode_flag_t::none },

    // 0xA0 - 0xAF (Segment pointer pushes/pops, CPUID, bit scanning operations)
    { "PUSH",      { "FS",    opaddr_t::S, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "POP",       { "FS",    opaddr_t::S, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "CPUID",     no_operand,                            no_operand,                             opcode_flag_t::none },
    { "BT",        { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "SHLD",      { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::none }, // 3rd operand implicit imm byte
    { "SHLD",      { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::none }, // 3rd operand implicit CL
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none },
    { "PUSH",      { "GS",    opaddr_t::S, opsize_t::w }, no_operand,                             opcode_flag_t::none }, // 0xA8
    { "POP",       { "GS",    opaddr_t::S, opsize_t::w }, no_operand,                             opcode_flag_t::none },
    { "RSM",       no_operand,                            no_operand,                             opcode_flag_t::none },
    { "BTS",       { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "SHRD",      { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "SHRD",      { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "GRP15",     { nullptr, opaddr_t::E, opsize_t::v }, no_operand,                             opcode_flag_t::none }, // FXSAVE, FXRSTOR, LFENCE...
    { "IMUL",      { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },

    // 0xB0 - 0xBF (CMPXCHG, Data conversions, Bit Tests)
    { "CMPXCHG",   { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "CMPXCHG",   { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "LSS",       { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::M, opsize_t::p }, opcode_flag_t::none },
    { "BTR",       { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "LFS",       { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::M, opsize_t::p }, opcode_flag_t::none },
    { "LGS",       { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::M, opsize_t::p }, opcode_flag_t::none },
    { "MOVZBL",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::b }, opcode_flag_t::none },
    { "MOVZWL",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::w }, opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none }, // 0xB8 (POPCNT space)
    { "UD1",       no_operand,                            no_operand,                             opcode_flag_t::none },
    { "GRP8",      { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::I, opsize_t::b }, opcode_flag_t::lockable | opcode_flag_t::group }, // Bit group extensions
    { "BTC",       { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable | opcode_flag_t::group },
    { "BSF",       { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "BSR",       { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::v }, opcode_flag_t::none },
    { "MOVSBL",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::b }, opcode_flag_t::none },
    { "MOVSWL",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::E, opsize_t::w }, opcode_flag_t::none },

    // 0xC0 - 0xCF (XADD, SIMD permutations, and multi-byte additions)
    { "XADD",      { nullptr, opaddr_t::E, opsize_t::b }, { nullptr, opaddr_t::G, opsize_t::b }, opcode_flag_t::lockable },
    { "XADD",      { nullptr, opaddr_t::E, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::lockable },
    { "CMPPS_OR_CMPPD",    { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "MOVNTI",    { nullptr, opaddr_t::M, opsize_t::v }, { nullptr, opaddr_t::G, opsize_t::v }, opcode_flag_t::none },
    { "PINSRW",    { nullptr, opaddr_t::P, opsize_t::q }, { nullptr, opaddr_t::E, opsize_t::w }, opcode_flag_t::none },
    { "PEXTRW",    { nullptr, opaddr_t::G, opsize_t::v }, { nullptr, opaddr_t::N, opsize_t::q }, opcode_flag_t::none },
    { "SHUFPS_OR_SHUFPD",  { nullptr, opaddr_t::V, opsize_t::ps }, { nullptr, opaddr_t::W, opsize_t::ps }, opcode_flag_t::none },
    { "GRP9",      { nullptr, opaddr_t::M, opsize_t::q }, no_operand,                             opcode_flag_t::lockable | opcode_flag_t::group }, // 0xC7 (CMPXCHG8B/16B)
    { "BSWAP",     { "rAX",   opaddr_t::A, opsize_t::v }, no_operand,                             opcode_flag_t::none }, // 0xC8
    { "BSWAP",     { "rCX",   opaddr_t::A, opsize_t::v }, no_operand,                             opcode_flag_t::none },
    { "BSWAP",     { "rDX",   opaddr_t::A, opsize_t::v }, no_operand,                             opcode_flag_t::none },
    { "BSWAP",     { "rBX",   opaddr_t::A, opsize_t::v }, no_operand,                             opcode_flag_t::none },
    { "BSWAP",     { "rSP",   opaddr_t::A, opsize_t::v }, no_operand,                             opcode_flag_t::none },
    { "BSWAP",     { "rBP",   opaddr_t::A, opsize_t::v }, no_operand,                             opcode_flag_t::none },
    { "BSWAP",     { "rSI",   opaddr_t::A, opsize_t::v }, no_operand,                             opcode_flag_t::none },
    { "BSWAP",     { "rDI",   opaddr_t::A, opsize_t::v }, no_operand,                             opcode_flag_t::none },

    // 0xD0 - 0xDF (SSE / MMX Packed Math Continued)
    { "ADDSUBPD_OR_ADDSUBPS", { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PAND",      { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PANDN",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "POR",       { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PXOR",      { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PADDB",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PADDW",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PADDD",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMAXUB",    { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none }, // 0xD8
    { "PMINUB",    { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMULHUW",   { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMULHW",    { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "CVTTPD2DQ_OR_CVTPI2PD", { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PINSRD_OR_PINSRQ",      { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::E, opsize_t::d  }, opcode_flag_t::none },
    { "PEXTRD_OR_PEXTRQ",      { nullptr, opaddr_t::E, opsize_t::d  }, { nullptr, opaddr_t::V, opsize_t::dq }, opcode_flag_t::none },
    { "PMULUDQ",   { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },

    // 0xE0 - 0xEF
    { "PAVGB",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSRAW",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSRAD",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMINSW",    { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMAXSW",    { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMADDWD",   { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSADBW",    { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "MASKMOVQ_OR_MASKMOVDQU", { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::N, opsize_t::q  }, opcode_flag_t::none },
    { "PSUBB",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none }, // 0xE8
    { "PSUBW",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSUBD",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PEXTRW",    { nullptr, opaddr_t::G, opsize_t::v  }, { nullptr, opaddr_t::V, opsize_t::dq }, opcode_flag_t::none },
    { "PMULLW",    { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMULILDQ",  { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "MOVDQ2Q",   { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::W, opsize_t::q  }, opcode_flag_t::none },
    { "MOVQ2DQ",   { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },

    // 0xF0 - 0xFF
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none }, // (LDDQU space)
    { "PSLLW",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSLLD",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSLLQ",     { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMULUDQ",   { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PMADDUBW",  { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PSUBUSB",   { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PSUBUSW",   { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMINSD",    { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none }, // 0xF8
    { "PMAXSD",    { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PEXTRW",    { nullptr, opaddr_t::E, opsize_t::w  }, { nullptr, opaddr_t::V, opsize_t::dq }, opcode_flag_t::none },
    { "PADDUSB",   { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PADDUSW",   { nullptr, opaddr_t::P, opsize_t::q  }, { nullptr, opaddr_t::Q, opsize_t::q  }, opcode_flag_t::none },
    { "PMAXUD",    { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "PMINUD",    { nullptr, opaddr_t::V, opsize_t::dq }, { nullptr, opaddr_t::W, opsize_t::dq }, opcode_flag_t::none },
    { "INVALID",   no_operand,                            no_operand,                             opcode_flag_t::none }
};

}
