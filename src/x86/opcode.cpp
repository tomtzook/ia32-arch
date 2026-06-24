
#include "x86/opcode.h"

namespace x86::opcode {

static register_t register_encoding[][4] = {
    { register_t::al, register_t::ax, register_t::eax, register_t::rax },
    { register_t::cl, register_t::cx, register_t::ecx, register_t::rcx },
    { register_t::dl, register_t::dx, register_t::edx, register_t::rdx },
    { register_t::bl, register_t::bx, register_t::eax, register_t::rax },
    { register_t::ah, register_t::sp, register_t::esp, register_t::rsp },
    { register_t::ch, register_t::bp, register_t::ebp, register_t::rbp },
    { register_t::dh, register_t::si, register_t::esi, register_t::rsi },
    { register_t::bh, register_t::di, register_t::edi, register_t::rdi },
};

static register_t register_extended_encoding[][4] = {
    { register_t::r8b, register_t::r8w, register_t::r8d, register_t::r8 },
    { register_t::r9b, register_t::r9w, register_t::r9d, register_t::r9 },
    { register_t::r10b, register_t::r10w, register_t::r10d, register_t::r10 },
    { register_t::r11b, register_t::r11w, register_t::r11d, register_t::r11 },
    { register_t::r12b, register_t::r12w, register_t::r12d, register_t::r12 },
    { register_t::r13b, register_t::r13w, register_t::r13d, register_t::r13 },
    { register_t::r14b, register_t::r14w, register_t::r14d, register_t::r14 },
    { register_t::r15b, register_t::r15w, register_t::r15d, register_t::r15 },
};

static const char* register_names[] = {
    "al", "ah", "ax", "eax", "rax",
    "bl", "bh", "bx", "ebx", "rbx",
    "cl", "ch", "cx", "ecx", "rcx",
    "dl", "dh", "dx", "edx", "rdx",
    "spl", "sp", "esp", "rsp",
    "bpl", "bp", "ebp", "rbp",
    "sil", "si", "esi", "rsi",
    "dil", "di", "edi", "rdi",
    "ip", "eip", "rip",
    "r8b", "r8w", "r8d", "r8",
    "r9b", "r9w", "r9d", "r9",
    "r10b", "r10w", "r10d", "r10",
    "r11b", "r11w", "r11d", "r11",
    "r12b", "r12w", "r12d", "r12",
    "r13b", "r13w", "r13d", "r13",
    "r14b", "r14w", "r14d", "r14",
    "r15b", "r15w", "r15d", "r15",
    "cs", "ds", "es", "fs", "gs", "ss"
};

constexpr auto last_register = register_t::ss;
constexpr auto last_register_encoding = register_encoding_t::edi;
constexpr auto last_ext_register_encoding = extended_register_encoding_t::r15;
constexpr auto last_addressing_size = addressing_size_t::qword;

register_t translate_register(const register_encoding_t encoding, const addressing_size_t size) {
    static_assert(static_cast<size_t>(last_register_encoding) == array_size(register_encoding) - 1, "register_encoding table missing rows");
    static_assert(static_cast<size_t>(last_addressing_size) == array_size(register_encoding[0]) - 1, "register_encoding table missing columns");
    return register_encoding[static_cast<size_t>(encoding)][static_cast<size_t>(size)];
}

register_t translate_register(const extended_register_encoding_t encoding, const addressing_size_t size) {
    if (encoding < extended_register_encoding_t::r8) {
        return translate_register(static_cast<register_encoding_t>(encoding), size);
    }

    static_assert(static_cast<size_t>(last_ext_register_encoding) - static_cast<size_t>(extended_register_encoding_t::r8) == array_size(register_extended_encoding) - 1, "register_extended_encoding table missing rows");
    static_assert(static_cast<size_t>(last_addressing_size) == array_size(register_extended_encoding[0]) - 1, "register_extended_encoding table missing columns");
    return register_extended_encoding[static_cast<size_t>(encoding) - static_cast<size_t>(extended_register_encoding_t::r8)][static_cast<size_t>(size)];
}

register_t translate_byte_rex_modified_register(const register_encoding_t encoding) {
    switch (encoding) {
        case register_encoding_t::eax:
            return register_t::al;
        case register_encoding_t::ecx:
            return register_t::cl;
        case register_encoding_t::edx:
            return register_t::dl;
        case register_encoding_t::ebx:
            return register_t::bl;
        case register_encoding_t::esp:
            return register_t::spl;
        case register_encoding_t::ebp:
            return register_t::bpl;
        case register_encoding_t::esi:
            return register_t::sil;
        case register_encoding_t::edi:
            return register_t::dil;
        default:
            __builtin_unreachable();
    }
}

const char* get_register_name(const register_t register_) {
    static_assert(static_cast<size_t>(last_register) == array_size(register_names) - 1, "register_names table missing entries");
    if (static_cast<size_t>(register_) >= array_size(register_names)) {
        return "";
    }
    return register_names[static_cast<size_t>(register_)];
}

}
