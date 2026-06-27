
#include "x86/opcode_decode.h"

namespace x86::opcode {

#define DECODE_TRY(...) \
    do { \
        const auto __code = __VA_ARGS__; \
        if (__code != decode_error_t::success) { \
            return __code; \
        }\
    } while (0)

struct decoder_context_t {
    const uint8_t* ptr;
    mode_t mode;
    decoded_opcode_t decoded;
    struct {
        bool has = false;
        mod_rm_t value;
    } modrm;
};

template<typename t_>
t_ read(decoder_context_t& context) {
    const auto value = *reinterpret_cast<const t_*>(context.ptr);
    context.ptr += sizeof(t_);
    return value;
}

static bool is_modrm_needed_by_operand(const tables::operand_t& def) {
    if (!def.exists) {
        return false;
    }

    switch (def.addressing) {
        case tables::opaddr_t::E:
        case tables::opaddr_t::G:
        case tables::opaddr_t::M:
            return true;

        default:
            return false;
    }
}

static bool is_modrm_expected(const tables::opcode_t& opcode) {
    return ((opcode.flags & tables::opcode_flag_t::group) == tables::opcode_flag_t::group) ||
        is_modrm_needed_by_operand(opcode.operand1) || is_modrm_needed_by_operand(opcode.operand2);
}

static size_t get_sib_scale_value(const sib_scale_t scale) {
    switch (scale) {
        case sib_scale_t::mult_1:
            return 1;
        case sib_scale_t::mult_2:
            return 2;
        case sib_scale_t::mult_4:
            return 4;
        case sib_scale_t::mult_8:
            return 8;
        default:
            return 0;
    }
}

static bool is_stack_op(const decoder_context_t& context) {
    /*switch (context.decoded.instruction) {
        case instruction_t::push:
        case instruction_t::pusha:
        case instruction_t::pushad:
        case instruction_t::pushfq:
        case instruction_t::pushfd:
        case instruction_t::pop:
        case instruction_t::popa:
        case instruction_t::popad:
        case instruction_t::popfq:
        case instruction_t::popfd:
        case instruction_t::popcnt:
            return true;
        default:
            return false;
    }*/
    return context.decoded.definition.is_stack_op();
}

static bool determine_group(const opcode_family_t family, const uint8_t opcode, opcode_group_t& group_out) {
    opcode_group_t group;
    switch (family) {
        case opcode_family_t::primary:
            switch (opcode) {
            case 0x80 ... 0x83:
                    group = opcode_group_t::group1;
                    break;
            case 0xc0 ... 0xc1:
            case 0xd0 ... 0xd3:
                    group = opcode_group_t::group2;
                    break;
            case 0xf6 ... 0xf7:
                    group = opcode_group_t::group3;
                    break;
            case 0xfe:
                    group = opcode_group_t::group4;
                    break;
            case 0xff:
                    group = opcode_group_t::group5;
                    break;
            case 0xc6 ... 0xc7:
                    group = opcode_group_t::group11;
                    break;
            default:
                    return false;
            }
            break;
        case opcode_family_t::extended_2byte:
            switch (opcode) {
            case 0x00:
                    group = opcode_group_t::group6;
                    break;
            case 0x01:
                    group = opcode_group_t::group7;
                    break;
            case 0xba:
                    group = opcode_group_t::group8;
                    break;
            case 0xc7:
                    group = opcode_group_t::group9;
                    break;
            case 0xae:
                    group = opcode_group_t::group15;
                    break;
            default:
                    return false;
            }
            break;
        default:
            return false;
    }

    group_out = group;
    return true;
}

static bool handle_prefixes(decoder_context_t& context, const uint8_t value) {
    switch (static_cast<legacy_prefix_t>(value)) {
        case legacy_prefix_t::lock:
            context.decoded.prefix.lock = true;
            break;
        case legacy_prefix_t::repne:
            context.decoded.prefix.repeat = repeat_type_t::repne;
            break;
        case legacy_prefix_t::rep:
            context.decoded.prefix.repeat = repeat_type_t::rep;
            break;
        case legacy_prefix_t::cs_seg_override:
            context.decoded.prefix.segment_override = segment_override_t::cs;
            break;
        case legacy_prefix_t::ss_seg_override:
            context.decoded.prefix.segment_override = segment_override_t::ss;
            break;
        case legacy_prefix_t::ds_seg_override:
            context.decoded.prefix.segment_override = segment_override_t::ds;
            break;
        case legacy_prefix_t::es_seg_override:
            context.decoded.prefix.segment_override = segment_override_t::es;
            break;
        case legacy_prefix_t::fs_seg_override:
            context.decoded.prefix.segment_override = segment_override_t::fs;
            break;
        case legacy_prefix_t::gs_seg_override:
            context.decoded.prefix.segment_override = segment_override_t::gs;
            break;
        case legacy_prefix_t::operand_size_override:
            context.decoded.prefix.operand_size = true;
            break;
        case legacy_prefix_t::address_size_override:
            context.decoded.prefix.address_size = true;
            break;
        case legacy_prefix_t::rex_prefix_range_min ... legacy_prefix_t::rex_prefix_range_max: {
            rex_prefix_t rex{};
            rex.raw = value;

            if (rex.bits.base) {
                context.decoded.prefix.rex.base = true;
            }
            if (rex.bits.index) {
                context.decoded.prefix.rex.index = true;
            }
            if (rex.bits.reg) {
                context.decoded.prefix.rex.reg = true;
            }
            if (rex.bits.width) {
                context.decoded.prefix.rex.width = true;
            }
            if ((rex.raw & (~static_cast<size_t>(legacy_prefix_t::rex_prefix_range_min))) > 0) {
                context.decoded.prefix.rex.present = true;
            }
            break;
        }
        default:
            return false;
    }

    // todo: separate segment vs branch hint prefix:
    //      depends on the opcode
    //          if it is branch then we want a branch hint
    //          if it is a memory access then we want segment override
    //      both are unused in x86-64

    return true;
}

static void handle_opcode(decoder_context_t& context) {
    auto family = opcode_family_t::primary;
    uint32_t full_opcode = 0; // full opcode can be 1 to 4 bytes
    uint8_t opcode_value = 0;

    const auto first_byte = *context.ptr;
    ++context.ptr;
    full_opcode |= first_byte;

    switch (first_byte) {
        case opcode_escape_2byte: {
            const auto second_byte = *context.ptr;
            ++context.ptr;
            full_opcode |= (second_byte << 8);

            switch (second_byte) {
                case opcode_escape_3byte_1: {
                    family = opcode_family_t::extended_3byte_1;
                    const auto third_byte = *context.ptr;
                    ++context.ptr;
                    full_opcode |= (third_byte << 16);
                    opcode_value = third_byte;
                    break;
                }
                case opcode_escape_3byte_2: {
                    family = opcode_family_t::extended_3byte_2;
                    const auto third_byte = *context.ptr;
                    ++context.ptr;
                    full_opcode |= (third_byte << 16);
                    opcode_value = third_byte;
                    break;
                }
                default:
                    family = opcode_family_t::extended_2byte;
                    opcode_value = second_byte;
                    break;
            }

            break;
        }
        case opcode_escape_vexmap_1: {
            family = opcode_family_t::vex_2byte;

            const auto second_byte = *context.ptr;
            ++context.ptr;
            full_opcode |= (second_byte << 8);
            opcode_value = second_byte;
            break;
        }
        case opcode_escape_vexmap_2: {
            family = opcode_family_t::vex_3byte;

            const auto second_byte = *context.ptr;
            ++context.ptr;
            const auto third_byte = *context.ptr;
            ++context.ptr;

            full_opcode |= (second_byte << 8) | (third_byte << 16);
            opcode_value = second_byte | (third_byte << 8);
            break;
        }
        case opcode_fpumap_range_min ... opcode_fpumap_range_max:
            family = opcode_family_t::fpu;
            full_opcode = first_byte;
            opcode_value = first_byte;
            break;
        default:
            family = opcode_family_t::primary;
            full_opcode = first_byte;
            opcode_value = first_byte;
            break;
    }

    context.decoded.family = family;
    context.decoded.full_opcode = full_opcode;
    context.decoded.opcode = opcode_value;
}

static decode_error_t determine_addressing_size(const decoder_context_t& context, const tables::opsize_t opsize, addressing_size_t& size_out) {
    auto size = addressing_size_t::byte;
    switch (opsize) {
        case tables::opsize_t::b:
            size = addressing_size_t::byte;
            break;
        case tables::opsize_t::c:
            size = context.decoded.prefix.operand_size ? addressing_size_t::word : addressing_size_t::byte;
            break;
        case tables::opsize_t::d:
            size = addressing_size_t::dword;
            break;
        case tables::opsize_t::q:
            size = addressing_size_t::qword;
            break;
        case tables::opsize_t::v:
            switch (context.mode) {
                case mode_t::real_mode:
                    size = context.decoded.prefix.operand_size ? addressing_size_t::dword : addressing_size_t::word;
                    break;
                case mode_t::protected_mode:
                    size = context.decoded.prefix.operand_size ? addressing_size_t::word : addressing_size_t::dword;
                    break;
                case mode_t::long_mode:
                    if (context.decoded.prefix.rex.width) {
                        size = addressing_size_t::qword;
                    } else if (context.decoded.prefix.operand_size) {
                        size = addressing_size_t::word;
                    } else {
                        // stack ops default to qword in long mode
                        size = is_stack_op(context) ? addressing_size_t::qword : addressing_size_t::dword;
                    }
                    break;
                default:
                    return decode_error_t::unsupported_hardware_mode;
            }
            break;
        case tables::opsize_t::w:
            size = addressing_size_t::word;
            break;
        case tables::opsize_t::y:
            switch (context.mode) {
                case mode_t::real_mode:
            case mode_t::protected_mode:
                    size = addressing_size_t::dword;
                    break;
                case mode_t::long_mode:
                    size = context.decoded.prefix.operand_size ? addressing_size_t::qword : addressing_size_t::dword;
                    break;
                default:
                    return decode_error_t::unsupported_hardware_mode;
            }
            break;
        case tables::opsize_t::z:
            switch (context.mode) {
            case mode_t::real_mode:
                    size = context.decoded.prefix.operand_size ? addressing_size_t::dword : addressing_size_t::word;
                    break;
            case mode_t::protected_mode:
                    size = context.decoded.prefix.operand_size ? addressing_size_t::word : addressing_size_t::dword;
                    break;
            case mode_t::long_mode:
                    if (context.decoded.prefix.rex.width) {
                        size = addressing_size_t::dword;
                    } else if (context.decoded.prefix.operand_size) {
                        size = addressing_size_t::word;
                    } else {
                        size = addressing_size_t::dword;
                    }
                    break;
            default:
                    return decode_error_t::unsupported_hardware_mode;
            }
            break;
        default:
            return decode_error_t::unsupported_operand_size;
    }

    size_out = size;
    return decode_error_t::success;
}

static decode_error_t read_operand_immediate(decoder_context_t& context, const addressing_size_t size, decoded_operand_t& operand_out) {
    decoded_operand_t result{};
    switch (size) {
        case addressing_size_t::byte:
            result.type = decoded_operand_type_t::immediate_byte;
            result.value.i_byte = read<uint8_t>(context);
            break;
        case addressing_size_t::word:
            result.type = decoded_operand_type_t::immediate_word;
            result.value.i_word = read<uint16_t>(context);
            break;
        case addressing_size_t::dword:
            result.type = decoded_operand_type_t::immediate_dword;
            result.value.i_dword = read<uint32_t>(context);
            break;
        case addressing_size_t::qword:
            result.type = decoded_operand_type_t::immediate_qword;
            result.value.i_qword = read<uint64_t>(context);
            break;
        default:
            return decode_error_t::unknown_operand_addressing_size;
    }

    operand_out = result;
    return decode_error_t::success;
}

static register_t translate_register_from_modrm(const decoder_context_t& context, const register_encoding_t encoding, const addressing_size_t size) {
    if (context.decoded.prefix.rex.present && size == addressing_size_t::byte) {
        return translate_byte_rex_modified_register(encoding);
    } else {
        return translate_register(encoding, size);
    }
}

static decode_error_t get_base_register_from_modrm(const decoder_context_t& context, const addressing_size_t size, const mod_rm_t& modrm, register_t& register_out) {
    register_t result;
    if (context.decoded.prefix.rex.base) {
        // using extend register encoding
        const auto reg = static_cast<extended_register_encoding_t>(modrm.bits.rm | (1 << 3));
        result = translate_register(reg, size);
    } else {
        const auto reg = static_cast<register_encoding_t>(modrm.bits.rm);
        result = translate_register_from_modrm(context, reg, size);
    }

    register_out = result;
    return decode_error_t::success;
}

static decode_error_t get_index_register_from_modrm(const decoder_context_t& context, const addressing_size_t size, const mod_rm_t& modrm, register_t& register_out) {
    register_t result;
    if (context.decoded.prefix.rex.index) {
        // using extend register encoding
        const auto reg = static_cast<extended_register_encoding_t>(modrm.bits.rm | (1 << 3));
        result = translate_register(reg, size);
    } else {
        const auto reg = static_cast<register_encoding_t>(modrm.bits.rm);
        result = translate_register_from_modrm(context, reg, size);
    }

    register_out = result;
    return decode_error_t::success;
}

static decode_error_t get_extra_register_from_modrm(const decoder_context_t& context, const addressing_size_t size, const mod_rm_t& modrm, register_t& register_out) {
    register_t result;
    if (context.decoded.prefix.rex.reg) {
        // using extend register encoding
        const auto reg = static_cast<extended_register_encoding_t>(modrm.bits.reg_opcode | (1 << 3));
        result = translate_register(reg, size);
    } else {
        const auto reg = static_cast<register_encoding_t>(modrm.bits.reg_opcode);
        result = translate_register_from_modrm(context, reg, size);
    }

    register_out = result;
    return decode_error_t::success;
}

static ssize_t read_modrm_displacement(decoder_context_t& context, const mod_rm_t& modrm) {
    switch (modrm.bits.mod) {
        case mod_type_t::memory_no_disp:
            return 0;
        case mod_type_t::memory_short_disp:
            return read<int8_t>(context);
        case mod_type_t::memory_long_disp:
            return context.mode == mode_t::real_mode ? read<int16_t>(context) : read<int32_t>(context);
        default:
            return 0;
    }
}

static decode_error_t handle_modrm_mem(decoder_context_t& context, const addressing_size_t addressing_size, decoded_operand_t& operand_out) {
    decoded_operand_t operand{};
    const auto& modrm = context.modrm.value;
    switch (context.mode) {
        case mode_t::real_mode: {
            const auto displacement = read_modrm_displacement(context, modrm);
            if (modrm.bits.rm < 4) {
                register_t reg1;
                register_t reg2;
                switch (modrm.bits.rm) {
                    case 0:
                        reg1 = register_t::bx;
                        reg2 = register_t::si;
                        break;
                    case 1:
                        reg1 = register_t::bx;
                        reg2 = register_t::si;
                        break;
                    case 2:
                        reg1 = register_t::bp;
                        reg2 = register_t::di;
                        break;
                    case 3:
                        reg1 = register_t::bp;
                        reg2 = register_t::di;
                        break;
                    default:
                        return decode_error_t::unknown_register_encoding;
                }

                operand.type = decoded_operand_type_t::memory_sum;
                operand.value.mem_sum.reg1 = reg1;
                operand.value.mem_sum.reg2 = reg2;
                operand.value.mem_sum.displacement = displacement;
            } else {
                register_t reg1;
                switch (modrm.bits.rm) {
                    case 4:
                        reg1 = register_t::si;
                        break;
                    case 5:
                        reg1 = register_t::di;
                        break;
                    case 6:
                        reg1 = register_t::bp;
                        break;
                    case 7:
                        reg1 = register_t::bx;
                        break;
                    default:
                        return decode_error_t::unknown_register_encoding;
                }

                operand.type = decoded_operand_type_t::memory;
                operand.value.mem.base = reg1;
                operand.value.mem.displacement = displacement;
            }

            break;
        }
        case mode_t::protected_mode:
        case mode_t::long_mode: {
            if (modrm.bits.rm == modrm_use_sib) {
                const auto sib = read<sib_t>(context);

                bool has_base;
                register_t base_reg;
                if (sib.base != sib_no_base_reg) {
                    DECODE_TRY(get_base_register_from_modrm(context, addressing_size, modrm, base_reg));
                    has_base = true;
                } else {
                    has_base = false;
                }

                bool has_index;
                register_t index_reg;
                if (sib.index != sib_no_index_reg) {
                    DECODE_TRY(get_index_register_from_modrm(context, addressing_size, modrm, index_reg));
                    has_index = true;
                } else {
                    has_index = false;
                }

                if (modrm.bits.mod == mod_type_t::memory_no_disp) {
                    if (!has_index) {
                        if (!has_base || base_reg == register_t::r13) {
                            operand.type = decoded_operand_type_t::memory_offset;
                            operand.value.mem_offset.displacement = read<int32_t>(context);
                        } else {
                            operand.type = decoded_operand_type_t::memory;
                            operand.value.mem.base = base_reg;
                            operand.value.mem.displacement = 0;
                        }
                    } else {
                        if (!has_base || base_reg == register_t::r13) {
                            operand.type = decoded_operand_type_t::memory_scaled2;
                            operand.value.mem_scaled2.index = index_reg;
                            operand.value.mem_scaled2.displacement = read<int32_t>(context);
                        } else {
                            operand.type = decoded_operand_type_t::memory_scaled;
                            operand.value.mem_scaled.base = base_reg;
                            operand.value.mem_scaled.index = index_reg;
                            operand.value.mem_scaled.displacement = 0;
                        }
                    }
                } else {
                    const auto displacement = read_modrm_displacement(context, modrm);

                    if (!has_index) {
                        operand.type = decoded_operand_type_t::memory;
                        operand.value.mem.base = base_reg;
                        operand.value.mem.displacement = displacement;
                    } else {
                        operand.type = decoded_operand_type_t::memory_scaled;
                        operand.value.mem_scaled.base = base_reg;
                        operand.value.mem_scaled.index = index_reg;
                        operand.value.mem_scaled.scale = get_sib_scale_value(sib.scale);
                        operand.value.mem_scaled.displacement = displacement;
                    }
                }
            } else if (modrm.bits.mod == mod_type_t::memory_no_disp && modrm.bits.rm == modrm_use_rip_relative) {
                const auto displacement = read<int32_t>(context);

                if (context.mode == mode_t::long_mode) {
                    operand.type = decoded_operand_type_t::memory;
                    operand.value.mem.base = context.decoded.prefix.address_size ? register_t::rip : register_t::eip;
                    operand.value.mem.displacement = displacement;
                } else {
                    operand.type = decoded_operand_type_t::memory_offset;
                    operand.value.mem_offset.displacement = displacement;
                }
            } else {
                register_t base_reg;
                DECODE_TRY(get_base_register_from_modrm(context, addressing_size, modrm, base_reg));
                const auto displacement = read_modrm_displacement(context, modrm);

                operand.type = decoded_operand_type_t::memory;
                operand.value.mem.base = base_reg;
                operand.value.mem.displacement = displacement;
            }
            break;
        }
        default:
            return decode_error_t::unsupported_hardware_mode;
    }

    operand_out = operand;
    return decode_error_t::success;
}

static decode_error_t handle_addressing_A(decoder_context_t& context, const tables::operand_t& def, const addressing_size_t addressing_size, decoded_operand_t& operand_out) {
    // operand info should be embedded in the opcode
    decoded_operand_t operand{};
    switch (def.embedded.type) {
        case tables::embedded_info_type_t::reg:
            operand.type = decoded_operand_type_t::reg;
            operand.value.reg = def.embedded.data.reg;
            break;
        case tables::embedded_info_type_t::reg_enc:
            operand.type = decoded_operand_type_t::reg;
            operand.value.reg = translate_register(def.embedded.data.reg_enc, addressing_size);
            break;
        case tables::embedded_info_type_t::memory:
            DECODE_TRY(read_operand_immediate(context, addressing_size, operand));
            break;
        case tables::embedded_info_type_t::const_int:
            operand.type = decoded_operand_type_t::immediate_dword;
            operand.value.i_dword = def.embedded.data.i;
            break;
        case tables::embedded_info_type_t::none:
        default:
            return decode_error_t::opcode_missing_embedded_data;
    }

    operand_out = operand;
    return decode_error_t::success;
}

static decode_error_t handle_addressing_E(decoder_context_t& context, const addressing_size_t addressing_size, decoded_operand_t& operand_out) {
    // mod + r/m
    decoded_operand_t operand{};
    const auto& modrm = context.modrm.value;
    if (modrm.bits.mod == mod_type_t::register_) {
        register_t register_;
        DECODE_TRY(get_base_register_from_modrm(context, addressing_size, modrm, register_));
        operand.type = decoded_operand_type_t::reg;
        operand.value.reg = register_;;
    } else {
        DECODE_TRY(handle_modrm_mem(context, addressing_size, operand));
    }

    operand_out = operand;
    return decode_error_t::success;
}

static decode_error_t handle_addressing_G(const decoder_context_t& context, const addressing_size_t addressing_size, decoded_operand_t& operand_out) {
    // modrm's reg field should contain our register
    decoded_operand_t operand{};
    if (context.decoded.definition.is_group()) {
        // reg_opcode bits indicate the instruction in the group
        operand.type = decoded_operand_type_t::none;
    } else {
        const auto& modrm = context.modrm.value;
        // reg_opcode bits indicate the source register
        register_t register_;
        DECODE_TRY(get_extra_register_from_modrm(context, addressing_size, modrm, register_));
        operand.type = decoded_operand_type_t::reg;
        operand.value.reg = register_;
    }

    operand_out = operand;
    return decode_error_t::success;
}

static decode_error_t handle_addressing_I(decoder_context_t& context, const addressing_size_t addressing_size, decoded_operand_t& operand_out) {
    // immediate value encoded in the byte stream
    decoded_operand_t operand{};
    DECODE_TRY(read_operand_immediate(context, addressing_size, operand));

    operand_out = operand;
    return decode_error_t::success;
}

static decode_error_t handle_addressing_J(decoder_context_t& context, const addressing_size_t addressing_size, decoded_operand_t& operand_out) {
    // basically meant for instructions that "jump" (also includes call). We should have a relative displacement
    // encoded here, so lets get it. The actually "jump" target is rip_after_this_instruction + displacement
    decoded_operand_t operand{};
    operand.type = decoded_operand_type_t::instruction_displacement;
    switch (addressing_size) {
        case addressing_size_t::byte:
            operand.value.instruct_displacement = read<int8_t>(context);
            break;
        case addressing_size_t::word:
            operand.value.instruct_displacement = read<int16_t>(context);
            break;
        case addressing_size_t::dword:
            operand.value.instruct_displacement = read<int32_t>(context);
            break;
        case addressing_size_t::qword:
            operand.value.instruct_displacement = read<int64_t>(context);
            break;
        default:
            return decode_error_t::unknown_operand_addressing_size;
    }

    operand_out = operand;
    return decode_error_t::success;
}

static decode_error_t handle_addressing_M(decoder_context_t& context, const addressing_size_t addressing_size, decoded_operand_t& operand_out) {
    // we should have modrm and it must refer to memory
    decoded_operand_t operand{};
    DECODE_TRY(handle_modrm_mem(context, addressing_size, operand));

    operand_out = operand;
    return decode_error_t::success;
}

static decode_error_t handle_addressing_O(decoder_context_t& context, const tables::operand_t& def, const addressing_size_t addressing_size, decoded_operand_t& operand_out) {
    // we should have an embedded register in the opcode definition.
    decoded_operand_t operand{};
    switch (def.embedded.type) {
        case tables::embedded_info_type_t::reg:
            operand.type = decoded_operand_type_t::reg;
            operand.value.reg = def.embedded.data.reg;
            break;
        case tables::embedded_info_type_t::reg_enc:
            operand.type = decoded_operand_type_t::reg;
            operand.value.reg = translate_register(def.embedded.data.reg_enc, addressing_size);
            break;
        case tables::embedded_info_type_t::none:
        default:
            return decode_error_t::opcode_missing_embedded_data;
    }

    operand_out = operand;
    return decode_error_t::success;
}

static decode_error_t handle_operand(decoder_context_t& context, const tables::operand_t& def, decoded_operand_t& operand_out) {
    addressing_size_t addressing_size;
    DECODE_TRY(determine_addressing_size(context, def.size, addressing_size));

    decoded_operand_t operand{};
    switch (def.addressing) {
        case tables::opaddr_t::A:
            DECODE_TRY(handle_addressing_A(context, def, addressing_size, operand));
            break;
        case tables::opaddr_t::E:
            DECODE_TRY(handle_addressing_E(context, addressing_size, operand));
            break;
        case tables::opaddr_t::G:
            DECODE_TRY(handle_addressing_G(context, addressing_size, operand));
            break;
        case tables::opaddr_t::I:
            DECODE_TRY(handle_addressing_I(context, addressing_size, operand));
            break;
        case tables::opaddr_t::J:
            DECODE_TRY(handle_addressing_J(context, addressing_size, operand));
            break;
        case tables::opaddr_t::M:
            DECODE_TRY(handle_addressing_M(context, addressing_size, operand));
            break;
        case tables::opaddr_t::O:
            DECODE_TRY(handle_addressing_O(context, def, addressing_size, operand));
            break;
        default:
            return decode_error_t::unknown_operand_addressing_mode;
    }

    operand_out = operand;
    return decode_error_t::success;
}

static decode_error_t decode(decoder_context_t& context) {
    // handle prefix
    do {
        const auto value = *context.ptr;
        if (handle_prefixes(context, value)) {
            ++context.ptr;
        } else {
            break;
        }
    } while (true);

    // handle base opcode
    handle_opcode(context);

    const auto def = tables::find_opcode(context.decoded.family, context.decoded.opcode);
    if (def == nullptr) {
        return decode_error_t::opcode_definition_not_found;
    }

    context.decoded.definition = *def;

    if (is_modrm_expected(context.decoded.definition)) {
        const auto* modrm = reinterpret_cast<const mod_rm_t*>(context.ptr);
        context.ptr += sizeof(mod_rm_t);
        context.modrm.has = true;
        context.modrm.value = *modrm;
    }

    if (context.decoded.definition.is_group()) {
        const auto value = context.modrm.value.bits.reg_opcode;
        const auto group = context.decoded.definition.kind.group;

        const auto def_group = tables::find_opcode_from_group(group, value);
        if (def_group == nullptr) {
            return decode_error_t::opcode_definition_not_found;
        }

        context.decoded.definition = tables::merge_group_opcode(&context.decoded.definition, def_group);
    }

    if (context.decoded.definition.is_pair()) {
        context.decoded.instruction = translate_instruction_pair(context.decoded.definition.kind.pair);
    } else {
        context.decoded.instruction = context.decoded.definition.kind.instruction;
    }

    if (context.decoded.definition.operand1.exists) {
        decoded_operand_t operand;
        DECODE_TRY(handle_operand(context, context.decoded.definition.operand1, operand));
        context.decoded.op1 = operand;
    } else {
        context.decoded.op1.type = decoded_operand_type_t::none;
    }

    if (context.decoded.definition.operand2.exists) {
        decoded_operand_t operand;
        DECODE_TRY(handle_operand(context, context.decoded.definition.operand2, operand));
        context.decoded.op2 = operand;
    } else {
        context.decoded.op2.type = decoded_operand_type_t::none;
    }

    return decode_error_t::success;
}

decode_result_t decode(const mode_t mode, const void* ptr) {
    decoder_context_t context{};
    context.ptr = static_cast<const uint8_t*>(ptr);
    context.mode = mode;

    const auto error = decode(context);
    if (error != decode_error_t::success) {
        return { false, error, context.ptr, {}};
    }

    return { true, decode_error_t::success, context.ptr, context.decoded};
}

}
