
#include "x86/opcode.h"

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

static bool is_modrm_needed_by_operand(const def::operand_t& def) {
    if (!def.exists) {
        return false;
    }

    switch (def.addressing) {
        case def::opaddr_t::E:
        case def::opaddr_t::G:
            return true;

        default:
            return false;
    }
}

static bool is_modrm_expected(const def::opcode_t& opcode) {
    return ((opcode.flags & def::opcode_flag_t::group) == def::opcode_flag_t::group) ||
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

static const def::opcode_t* find_opcode_def(const opcode_family_t family, const uint8_t value) {
    if (value >= def::table_size) {
        return nullptr;
    }

    const def::opcode_t* def;
    switch (family) {
        case opcode_family_t::primary:
            def = &def::table_primary[value];
            break;
        case opcode_family_t::extended_2byte:
            def = &def::table_extended_2byte[value];
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

    // todo: check it is not a placeholder
    return def;
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

static const def::opcode_t* find_opcode_def_for_group(const opcode_group_t group, const uint8_t value) {
    if (value >= def::group_table_size) {
        return nullptr;
    }

    const def::opcode_t* def = nullptr;
    switch (group) {
        case opcode_group_t::group1:
            def = &def::table_group_1[value];
            break;
        case opcode_group_t::group2:
            def = &def::table_group_2[value];
            break;
        case opcode_group_t::group3:
            def = &def::table_group_3[value];
            break;
        case opcode_group_t::group4:
            def = &def::table_group_4[value];
            break;
        case opcode_group_t::group5:
            def = &def::table_group_5[value];
            break;
        case opcode_group_t::group6:
            def = &def::table_group_6[value];
            break;
        case opcode_group_t::group7:
            def = &def::table_group_7[value];
            break;
        case opcode_group_t::group8:
            def = &def::table_group_8[value];
            break;
        case opcode_group_t::group9:
            def = &def::table_group_9[value];
            break;
        case opcode_group_t::group11:
            def = &def::table_group_11[value];
            break;
        case opcode_group_t::group15:
            def = &def::table_group_15[value];
            break;
    }

    if (def == nullptr || !def->exists) {
        return nullptr;
    }

    // todo: check it is not a placeholder
    return def;
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

static decode_error_t determine_addressing_size(const decoder_context_t& context, const def::opsize_t opsize, addressing_size_t& size_out) {
    auto size = addressing_size_t::byte;
    switch (opsize) {
        case def::opsize_t::b:
            size = addressing_size_t::byte;
            break;
        case def::opsize_t::c:
            size = context.decoded.prefix.operand_size ? addressing_size_t::word : addressing_size_t::byte;
            break;
        case def::opsize_t::d:
            size = addressing_size_t::dword;
            break;
        case def::opsize_t::q:
            size = addressing_size_t::qword;
            break;
        case def::opsize_t::v:
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
                        size = addressing_size_t::dword;
                    }
                    break;
                default:
                    return decode_error_t::unsupported_hardware_mode;
            }
            break;
        case def::opsize_t::w:
            size = addressing_size_t::word;
            break;
        case def::opsize_t::y:
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
        case def::opsize_t::z:
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

static decode_error_t get_base_register_from_modrm(const decoder_context_t& context, const addressing_size_t size, const mod_rm_t& modrm, register_t& register_out) {
    register_t result;
    if (context.decoded.prefix.rex.base) {
        // using extend register encoding
        const auto reg = static_cast<extended_register_encoding_t>(modrm.bits.rm | (1 << 4));
        result = translate_register(reg, size);
    } else {
        const auto reg = static_cast<register_encoding_t>(modrm.bits.rm);
        result = translate_register(reg, size);
    }

    register_out = result;
    return decode_error_t::success;
}

static decode_error_t get_index_register_from_modrm(const decoder_context_t& context, const addressing_size_t size, const mod_rm_t& modrm, register_t& register_out) {
    register_t result;
    if (context.decoded.prefix.rex.index) {
        // using extend register encoding
        const auto reg = static_cast<extended_register_encoding_t>(modrm.bits.rm | (1 << 4));
        result = translate_register(reg, size);
    } else {
        const auto reg = static_cast<register_encoding_t>(modrm.bits.rm);
        result = translate_register(reg, size);
    }

    register_out = result;
    return decode_error_t::success;
}

static decode_error_t get_extra_register_from_modrm(const decoder_context_t& context, const addressing_size_t size, const mod_rm_t& modrm, register_t& register_out) {
    register_t result;
    if (context.decoded.prefix.rex.reg) {
        // using extend register encoding
        const auto reg = static_cast<extended_register_encoding_t>(modrm.bits.reg_opcode | (1 << 4));
        result = translate_register(reg, size);
    } else {
        const auto reg = static_cast<register_encoding_t>(modrm.bits.reg_opcode);
        result = translate_register(reg, size);
    }

    register_out = result;
    return decode_error_t::success;
}

static size_t read_modrm_displacement(decoder_context_t& context, const mod_rm_t& modrm) {
    switch (modrm.bits.mod) {
        case mod_type_t::memory_no_disp:
            return 0;
        case mod_type_t::memory_short_disp:
            return read<uint8_t>(context);
        case mod_type_t::memory_long_disp:
            return context.mode == mode_t::real_mode ? read<uint16_t>(context) : read<uint32_t>(context);
        default:
            return 0;
    }
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
        // memory based
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

                    register_t base_reg;
                    DECODE_TRY(get_base_register_from_modrm(context, addressing_size, modrm, base_reg));
                    register_t index_reg;
                    DECODE_TRY(get_index_register_from_modrm(context, addressing_size, modrm, index_reg));

                    if (modrm.bits.mod == mod_type_t::memory_no_disp) {
                        if (index_reg == register_t::rsp) {
                            if (base_reg == register_t::rbp || base_reg == register_t::r13) {
                                operand.type = decoded_operand_type_t::memory_offset;
                                operand.value.mem_offset.displacement = read<uint32_t>(context);
                            } else {
                                operand.type = decoded_operand_type_t::memory;
                                operand.value.mem.base = base_reg;
                                operand.value.mem.displacement = 0;
                            }
                        } else {
                            if (base_reg == register_t::rbp || base_reg == register_t::r13) {
                                operand.type = decoded_operand_type_t::memory_scaled2;
                                operand.value.mem_scaled2.index = index_reg;
                                operand.value.mem_scaled2.displacement = read<uint32_t>(context);
                            } else {
                                operand.type = decoded_operand_type_t::memory_scaled;
                                operand.value.mem_scaled.base = base_reg;
                                operand.value.mem_scaled.index = index_reg;
                                operand.value.mem_scaled.displacement = 0;
                            }
                        }
                    } else {
                        const auto displacement = read_modrm_displacement(context, modrm);

                        if (index_reg == register_t::rsp) {
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
                    const auto displacement = read<uint32_t>(context);

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
    }

    operand_out = operand;
    return decode_error_t::success;
}

static decode_error_t handle_operand(decoder_context_t& context, const def::operand_t& def, decoded_operand_t& operand_out) {
    addressing_size_t addressing_size;
    DECODE_TRY(determine_addressing_size(context, def.size, addressing_size));

    decoded_operand_t operand{};
    switch (def.addressing) {
        case def::opaddr_t::A: {
            switch (def.embedded.type) {
                case def::embedded_info_type_t::reg:
                    operand.type = decoded_operand_type_t::reg;
                    operand.value.reg = def.embedded.data.reg;
                    break;
                case def::embedded_info_type_t::reg_enc:
                    operand.type = decoded_operand_type_t::reg;
                    operand.value.reg = translate_register(def.embedded.data.reg_enc, addressing_size);
                    break;
                case def::embedded_info_type_t::memory:
                    DECODE_TRY(read_operand_immediate(context, addressing_size, operand));
                    break;
                case def::embedded_info_type_t::const_int:
                    operand.type = decoded_operand_type_t::immediate_dword;
                    operand.value.i_dword = def.embedded.data.i;
                    break;
                case def::embedded_info_type_t::none:
                default:
                    return decode_error_t::opcode_missing_embedded_data;
            }
            break;
        }
        case def::opaddr_t::E: {
            DECODE_TRY(handle_addressing_E(context, addressing_size, operand));
            break;
        }
        case def::opaddr_t::G: {
            // modrm reg
            if ((context.decoded.definition.flags & def::opcode_flag_t::group) == def::opcode_flag_t::group) {
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
            break;
        }
        case def::opaddr_t::I: {
            DECODE_TRY(read_operand_immediate(context, addressing_size, operand));
            break;
        }
        case def::opaddr_t::O: {
            switch (def.embedded.type) {
                case def::embedded_info_type_t::reg:
                    operand.type = decoded_operand_type_t::reg;
                    operand.value.reg = def.embedded.data.reg;
                    break;
                case def::embedded_info_type_t::none:
                default:
                    return decode_error_t::opcode_missing_embedded_data;
            }
            break;
        }
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

    const auto def = find_opcode_def(context.decoded.family, context.decoded.opcode);
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

    if ((context.decoded.definition.flags & def::opcode_flag_t::group) == def::opcode_flag_t::group) {
        const auto value = context.modrm.value.bits.reg_opcode;
        opcode_group_t group;
        if (!determine_group(context.decoded.family, context.decoded.opcode, group)) {
            return decode_error_t::opcode_group_is_unknown;
        }

        const auto def_group = find_opcode_def_for_group(group, value);
        if (def_group == nullptr) {
            return decode_error_t::opcode_definition_not_found;
        }

        // merge definitions based on what the group contains
        if (!context.decoded.definition.operand1.exists && def_group->operand1.exists) {
            context.decoded.definition = *def_group;
        } else {
            context.decoded.definition.mnemonic = def_group->mnemonic;
        }
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
