
#include "x86/opcode.h"

namespace x86::opcode {

static register_t register_encoding[][4] = {
    { register_t::al, register_t::ax, register_t::eax, register_t::rax },
    { register_t::cl, register_t::cx, register_t::ecx, register_t::rcx },
    { register_t::dl, register_t::dx, register_t::edx, register_t::rdx },
    { register_t::bl, register_t::bx, register_t::eax, register_t::rax },
    { register_t::spl, register_t::sp, register_t::esp, register_t::rsp },
    { register_t::bpl, register_t::bp, register_t::ebp, register_t::rbp },
    { register_t::sil, register_t::si, register_t::esi, register_t::rsi },
    { register_t::dil, register_t::di, register_t::edi, register_t::rdi },
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
};


register_t translate_register(const register_encoding_t encoding, const addressing_size_t size) {
    // todo: safety checks
    // todo: high low in byte
    return register_encoding[static_cast<size_t>(encoding)][static_cast<size_t>(size)];
}

register_t translate_register(const extended_register_encoding_t encoding, const addressing_size_t size) {
    if (encoding < extended_register_encoding_t::r8) {
        // todo: safety checks
        // todo: high low in byte
        return register_encoding[static_cast<size_t>(encoding)][static_cast<size_t>(size)];
    }

    return register_extended_encoding[static_cast<size_t>(encoding)][static_cast<size_t>(size)];
}

const char* get_register_name(const register_t register_) {
    // todo: safety checks
    return register_names[static_cast<size_t>(register_)];
}

}
