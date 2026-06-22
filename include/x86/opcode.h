#pragma once

#include "types.h"

namespace x86::opcode {

namespace def {

enum class opcode_flag_t {
    none = 0,
    lockable,
    group,
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

struct operand_t {
    const char* mnemonic;
    opaddr_t addressing;
    opsize_t size;
    bool exists = true;
};

struct opcode_t {
    const char* mnemonic;
    operand_t operand1;
    operand_t operand2;
    opcode_flag_t flags;
};

static constexpr auto table_size = 256;
extern const opcode_t table_primary[table_size];
extern const opcode_t table_extended_2byte[table_size];

}

enum class mode_t {
    real_mode,
    protected_mode,
    long_mode
};

enum class legacy_prefix_t : uint8_t {
    // 1 byte
    // group 1
    lock = 0xf0,
    repne = 0xf2,
    rep = 0xf3,
    // group 2
    cs_seg_override = 0x2e,
    ss_seg_override = 0x36,
    ds_seg_override = 0x3e,
    es_seg_override = 0x26,
    fs_seg_override = 0x64,
    gs_seg_override = 0x65,
    branch_not_taken = 0x2e,
    branch_taken = 0x3e,
    // group 3
    operand_size_override = 0x66,
    // group 4
    address_size_override = 0x67,
    // rex
    rex_prefix_range_min = 0x40, // rex is for x64 only
    rex_prefix_range_max = 0x4f
};

enum class register_encoding_t : uint8_t {
    // 3 bits
    // 32bit, 64bit, 16bit, 8bit(high/low -> rex prefix)
    eax = 0, // eax, rax, ax, al
    ecx = 1, // ecx, rcx, cx, cl
    edx = 2, // edx, rdx, dx, dl
    ebx = 3, // ebx, rbx, bx, bl
    esp = 4, // esp, rsp, sp, ah/spl
    ebp = 5, // ebp, rbp, bp, ch/bpl
    esi = 6, // esi, rsi, si, dh/sil
    edi = 7, // edi, rdi, di, bh/dil
};

enum class extended_register_encoding_t : uint8_t {
    // only x86-64, 4 bits
    rax = 0,
    rcx = 1,
    rdx = 2,
    rbx = 3,
    rsp = 4,
    rbp = 5,
    rsi = 6,
    rdi = 7,
    r8 = 8,
    r9 = 9,
    r10 = 10,
    r11 = 11,
    r12 = 12,
    r13 = 13,
    r14 = 14,
    r15 = 15,
};

enum class segment_register_encoding_t : uint8_t {
    // 3 bits
    es = 0,
    cs = 1,
    ss = 2,
    ds = 3,
    fs = 4,
    gs = 5,
};

enum class control_register_encoding_t : uint8_t {
    // 4 bits
    cr0 = 0,
    cr2 = 2,
    cr3 = 3,
    cr4 = 4,
    cr8 = 8
};

enum class mod_type_t : uint8_t {
    // 2 bits
    memory_no_disp = 0,
    memory_short_disp = 1,
    memory_long_disp = 2,
    register_ = 3
};

enum class sib_scale_t : uint8_t {
    // 2 bits
    mult_1 = 0b00,
    mult_2 = 0b01,
    mult_4 = 0b10,
    mult_8 = 0b11,
};

enum class opcode_1byte_t : uint8_t {
    add_reg_to_8bit = 0x0,
    add_reg_to = 0x1,
    add_to_reg_8bit = 0x2,
    add_to_reg = 0x3,
    and_8bit = 0x20,
    and_ = 0x21,
    sub_8bit = 0x28,
    sub = 0x29,
    xor_8bit = 0x30,
    xor_ = 0x31,
    cmp_8bit = 0x38,
    cmp = 0x39,
    push_eax = 0x50,
    push_ecx = 0x51,
    push_edx = 0x52,
    push_ebx = 0x53,
    push_esp = 0x54,
    push_ebp = 0x55,
    push_esi = 0x56,
    push_edi = 0x57,
    pop_eax = 0x58,
    pop_ecx = 0x59,
    pop_edx = 0x5a,
    pop_ebx = 0x5b,
    pop_esp = 0x5c,
    pop_ebp = 0x5d,
    pop_esi = 0x5e,
    pop_edi = 0x5f,
    push_immediate_32bit = 0x68,
    push_immediate_8bit = 0x6a,
    jmp_short_if_of = 0x70,
    jmp_short_if_cf = 0x72,
    jmp_short_if_zf = 0x74,
    jmp_short_if_not_zf = 0x75,
    jmp_short_if_less = 0x7c,
    jmp_short_if_less_or_equal = 0x7e,
    mov_reg_to_8bit = 0x88,
    mov_reg_to = 0x89,
    mov_to_reg_8bit = 0x8a,
    mov_to_reg = 0x8b,
    mov_seg_to = 0x8c,
    mov_to_seg = 0x8e,
    nop = 0x90,
    mov_byte_to_al = 0xa0,
    mov_word_to_eax = 0xa1,
    mov_immediate_byte_to_al = 0xb0,
    mov_immediate_byte_to_cl = 0xb1,
    mov_immediate_byte_to_dl = 0xb2,
    mov_immediate_byte_to_bl = 0xb3,
    mov_immediate_byte_to_ah = 0xb4,
    mov_immediate_byte_to_ch = 0xb5,
    mov_immediate_byte_to_dh = 0xb6,
    mov_immediate_byte_to_bh = 0xb7,
    mov_immediate_to_eax = 0xb8,
    mov_immediate_to_ecx = 0xb9,
    mov_immediate_to_edx = 0xba,
    mov_immediate_to_ebx = 0xbb,
    mov_immediate_to_esp = 0xbc,
    mov_immediate_to_ebp = 0xbd,
    mov_immediate_to_esi = 0xbe,
    mov_immediate_to_edi = 0xbf,
    ret = 0xc3,
    mov_immediate_byte_to = 0xc6,
    mov_immediate_to = 0xc7,
    int3 = 0xcc,
    int_soft = 0xcd,
    iret = 0xcf,
    call_relative = 0xe8,
    jmp_near_relative = 0xe9,
    jmp_short = 0xeb,
    hlt = 0xf4
};

// groups = specific instruction defined by reg/opcode field of mod r/m
enum class opcode_2byte_t : uint8_t {
    // with escape 0F
    group6 = 0x0,
    group7 = 0x1,
    syscall = 0x5,
    clts = 0x6,
    sysret = 0x7,
    invd = 0x8,
    wbinvd = 0x9,
    ud2 = 0xb,
    mov_from_cr = 0x20,
    mov_from_dr = 0x21,
    mov_to_cr = 0x22,
    mov_to_dr = 0x23,
    wrmsr = 0x30,
    rdtsc = 0x31,
    rdmsr = 0x32,
    rdpmc = 0x33,
    cmovo = 0x40,
    cmovno = 0x41,
    cmovb = 0x42, // cmovnae
    cmovae = 0x43, // cmovnb
    cmove = 0x44, // cmovz
    cmovne = 0x45, //cmovnz
    cmovbe = 0x46, // cmovna
    cmova = 0x47, // cmovnbe
    cmovs = 0x48,
    cmovns = 0x49,
    cmovp = 0x4a, // cmovpe
    cmovnp = 0x4b, // cmovpo
    cmovl = 0x4c, // cmovnge
    cmovge = 0x4d, // cmovnl
    cmovle = 0x4e, // cmovng
    cmovg = 0x4f, // cmovnle
    vmread = 0x78,
    vmwrite = 0x79,
    jo = 0x80,
    jno = 0x81,
    jb = 0x82, // jnae / jc
    jnb = 0x83, // jae / jnc
    je = 0x84, // jz
    jne = 0x85, // jnz
    jbe = 0x86, // jna
    jnbe = 0x87, // ja
    js = 0x88,
    jns = 0x89,
    jp = 0x8a, // jpe
    jpo = 0x8b, // jnp
    jl = 0x8c, // jnge
    jnl = 0x8d, // jge
    jle = 0x8e, // jng
    jnle = 0x8f, // jg
    seto = 0x90,
    setno = 0x91,
    setb = 0x92, // setnae
    setnb = 0x93, // setae
    sete = 0x94, // setz
    setne = 0x95, // setnz
    setbe = 0x96, // setna
    setnbe = 0x97, // seta
    sets = 0x98,
    setns = 0x99,
    setp = 0x9a, // setpe
    setnp = 0x9b, // setpo
    setl = 0x9c, // setnge
    setnl = 0x9d, // setge
    setle = 0x9e, // setng
    setnle = 0x9f, // setg
    cpuid = 0xa2,
    group15 = 0xae,
    imul = 0xaf,
    bts = 0xab,
    cmpxchg_small = 0xb0,
    cmpxchg = 0xb1,
    movzx_small = 0xb6, // byte/word
    movzx = 0xb7, // dword/qword
    movsx_small = 0xbe, // byte/word
    movsx = 0xbf, // dword/qword
    xadd_8bit = 0xc0,
    xadd = 0xc1,
    group9 = 0xcf,
    bswap_eax = 0xc8,
    bswap_ecx = 0xc9,
    bswap_edx = 0xca,
    bswap_ebx = 0xcb,
    bswap_esp = 0xcc,
    bswap_ebp = 0xcd,
    bswap_esi = 0xce,
    bswap_edi = 0xcf
};

enum class group6_register_encoding_t : uint8_t {
    // 3 bits
    sldt = 0,
    str = 1,
    lldt = 2,
    ltr = 3,
    verr = 4,
    verw = 5
};

enum class group7_register_encoding_t : uint8_t {
    // 3 bits
    sgdt = 0,
    sidt = 1,
    lgdt = 2,
    lidt = 3,
    msync = 4,
    reserved = 5,
    lmsw = 6,
    invlpg = 7
};

enum class group7_extended_register_encoding_t : uint8_t {
    // when mod=11
    // full ModR/M byte
    vmcall = 0xc1,
    vmlaunch = 0xc2,
    vmresume = 0xc3,
    monitor = 0xc8,
    mwait = 0xc9,
    xgetbv = 0xd0,
    xsetbv = 0xd1
};

enum class group15_register_encoding_t : uint8_t {
    // 3 bits
    fxsave = 0,
    fxstor = 1,
    ldmxcsr = 2,
    stmxcsr = 3,
    lfence = 5,
    mfence = 6,
    sfence = 7
};

enum class group9_register_encoding_t : uint8_t {
    // 3 bits
    chmpxchg8b = 0,
    chmpxchg16b = 1,
    vmptrld = 3,
    vmptrst = 4,
    vmclear = 5,
    vmxon = 6,
    rdrand = 7
};

enum class opcode_family_t {
    primary = 0,
    extended_2byte,
    extended_3byte_1,
    extended_3byte_2,
    fpu,
    vex_2byte,
    vex_3byte,
    evex,
};

static constexpr uint8_t rex_prefix_range_min = 0x40; // rex is for x64 only
static constexpr uint8_t rex_prefix_range_max = 0x4f;

static constexpr auto modrm_use_sib = 4; // when mod is memory
static constexpr auto modrm_use_rip_relative = 5; // when mod=0

static constexpr auto sib_no_base_reg = register_encoding_t::ebp; // only if modr/m is 0
static constexpr auto sib_no_index_reg = register_encoding_t::esp;

static constexpr auto opcode_escape_2byte = 0x0f;
static constexpr auto opcode_escape_3byte_1 = 0x38; // 0x0f 0x38
static constexpr auto opcode_escape_3byte_2 = 0x3a; // 0x0f 0x3a
static constexpr auto opcode_fpumap_range_min = 0xd8;
static constexpr auto opcode_fpumap_range_max = 0xdf;
static constexpr auto opcode_escape_vexmap_1 = 0xc4; // 2 bytes
static constexpr auto opcode_escape_vexmap_2 = 0xc5; // 3 bytes
static constexpr auto opcode_escape_evexmap = 0x62;

enum class repeat_type_t : uint8_t {
    none = 0,
    rep,
    repne
};

enum class segment_override_t : uint8_t {
    none = 0,
    cs,
    ds,
    ss,
    es,
    fs,
    gs
};

enum class branch_hint_t : uint8_t {
    none = 0,
    taken,
    not_taken,
};

enum class decoded_operand_type_t : uint8_t {
    none,
    immediate_byte,
    immediate_word,
    immediate_dword,
    immediate_qword,
    reg,
    memory,
    memory_offset,
    memory_scaled,
    memory_scaled2,
    memory_sum,
};

enum class register_t {
    rax,
    rbx,
    rcx,
    rdx,
    rsp,
    rbp,
    rsi,
    rdi,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
    rip,
};

enum class addressing_size_t {
    byte,
    word,
    dword,
    qword
};

struct decoded_operand_t {
    decoded_operand_type_t type;
    union {
        uint8_t i_byte;
        uint8_t i_word;
        uint8_t i_dword;
        uint8_t i_qword;
        struct {
            register_t type;
            addressing_size_t size;
        } reg;
        struct {
            register_t base;
            addressing_size_t size;
            size_t displacement;
        } mem;
        struct {
            size_t displacement;
        } mem_offset;
        struct {
            register_t base;
            register_t index;
            addressing_size_t size;
            size_t scale;
            size_t displacement;
        } mem_scaled;
        struct {
            register_t index;
            addressing_size_t size;
            size_t scale;
            size_t displacement;
        } mem_scaled2;
        struct {
            register_t reg1;
            register_t reg2;
            addressing_size_t size;
            size_t displacement;
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
            bool base;
            bool index;
            bool reg;
            bool width;
        } rex;
    } prefix;
    struct {
        opcode_family_t family;
        uint8_t value;
        uint32_t full;
        def::opcode_t definition;
    } opcode;

    decoded_operand_t first;
    decoded_operand_t second;
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
    opcode_definition_not_found
};

struct decode_result_t {
    bool success;
    decode_error_t error_code;
    const void* ptr;
    decoded_opcode_t opcode;
};

#pragma pack(push, 1)

struct rex_prefix_t {
    union {
        struct {
            uint8_t base : 1;
            uint8_t index : 1;
            uint8_t reg : 1;
            uint8_t width : 1;
            uint8_t reserved0 : 4;
        } bits;
        uint8_t raw;
    };
};

struct mod_rm_t {
    // when mod is memory
    //  reg/opcode = standard register
    //  r/m = standard register containing base address [0x5 means rip relative (64bit) or 32 bit displacement]
    //      special values
    //          r/m = 4 -> use SIB byte for addressing
    //          r/m = 5 (mod=0) do rip relative (in 64bit) or 32 bit displacement
    union {
        struct {
            uint8_t rm : 3;
            uint8_t reg_opcode : 3;
            mod_type_t mod : 2;
        } bits;
        uint8_t raw;
    };
};

struct sib_t {
    union {
        struct {
            register_encoding_t base : 3;
            register_encoding_t index : 3;
            sib_scale_t scale : 2;
        };
        uint8_t raw;
    };
};

#pragma pack(pop)

decode_result_t decode(mode_t mode, const void* ptr);

}
