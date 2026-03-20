
#include "x86/interrupts.h"


namespace x86::interrupts {

uint32_t descriptor_t::address() const {
    return bits.offset_low | (static_cast<uint32_t>(bits.offset_high) << 16);
}

void descriptor_t::address(const uint32_t address) {
    bits.offset_low = address & 0xffff;
    bits.offset_high = (address >> 16) & 0xffff;
}

uint64_t descriptor64_t::address() const {
    return static_cast<uint64_t>(low.bits.offset_low) |
        (static_cast<uint64_t>(low.bits.offset_high) << 16) |
            (static_cast<uint64_t>(high.bits.offset_upper) << 32);
}

void descriptor64_t::address(const uint64_t address) {
    low.bits.offset_low = address & 0xffff;
    low.bits.offset_high = (address >> 16) & 0xffff;
    high.bits.offset_upper = (address >> 32) & 0xffff;
}

table64_t::table64_t(const idtr_t idtr)
    : m_idtr(idtr)
{}

const void* table64_t::base_address() const {
    return reinterpret_cast<const void *>(m_idtr.base_address);
}

void* table64_t::base_address() {
    return reinterpret_cast<void*>(m_idtr.base_address);
}

size_t table64_t::limit() const {
    return m_idtr.limit;
}

size_t table64_t::count() const {
    return (limit() + 1) / sizeof(descriptor64_t);
}

const descriptor64_t& table64_t::operator[](const size_t index) const {
    return static_cast<const descriptor64_t*>(base_address())[index];
}

descriptor64_t& table64_t::operator[](const size_t index) {
    return static_cast<descriptor64_t*>(base_address())[index];
}

const descriptor64_t& table64_t::operator[](interrupt_t interrupt) const {
    return static_cast<const descriptor64_t*>(base_address())[static_cast<uint16_t>(interrupt)];
}

descriptor64_t& table64_t::operator[](interrupt_t interrupt) {
    return static_cast<descriptor64_t*>(base_address())[static_cast<uint16_t>(interrupt)];
}

interrupt_type_t vector_type(const interrupt_t vector) {
    switch (vector) {
        case interrupt_t::divide_error:
        case interrupt_t::debug_exception:
        case interrupt_t::bound_range_exceeded:
        case interrupt_t::invalid_opcode:
        case interrupt_t::device_not_available:
        case interrupt_t::coprocessor_segment_overrun:
        case interrupt_t::invalid_tss:
        case interrupt_t::segment_not_present:
        case interrupt_t::stack_segment_fault:
        case interrupt_t::general_protection:
        case interrupt_t::page_fault:
        case interrupt_t::fpu_floating_point_error:
        case interrupt_t::alignment_check:
        case interrupt_t::simd_floating_point_exception:
        case interrupt_t::virtualization_exception:
        case interrupt_t::control_protection:
        case interrupt_t::hypervisor_injection:
        case interrupt_t::vmm_communication:
        case interrupt_t::security_exception:
            return interrupt_type_t::fault;
        case interrupt_t::nmi:
            return interrupt_type_t::interrupt;
        case interrupt_t::breakpoint:
        case interrupt_t::overflow:
            return interrupt_type_t::trap;
        case interrupt_t::double_fault:
        case interrupt_t::machine_check:
            return interrupt_type_t::abort;
        default:
            __builtin_unreachable();
    }
}

const char* vector_to_str(const interrupt_t vector) {
    switch (vector) {
        case interrupt_t::divide_error: return "#DV";
        case interrupt_t::debug_exception: return "#DB";
        case interrupt_t::nmi: return "NMI";
        case interrupt_t::breakpoint: return "#BP";
        case interrupt_t::overflow: return "#OF";
        case interrupt_t::bound_range_exceeded: return "#BR";
        case interrupt_t::invalid_opcode: return "#UD";
        case interrupt_t::device_not_available: return "#NM";
        case interrupt_t::double_fault: return "#DF";
        case interrupt_t::coprocessor_segment_overrun: return "";
        case interrupt_t::invalid_tss: return "#TS";
        case interrupt_t::segment_not_present: return "#NP";
        case interrupt_t::stack_segment_fault: return "#SS";
        case interrupt_t::general_protection: return "#GP";
        case interrupt_t::page_fault: return "#PF";
        case interrupt_t::fpu_floating_point_error: return "#MF";
        case interrupt_t::alignment_check: return "#AC";
        case interrupt_t::machine_check: return "#MC";
        case interrupt_t::simd_floating_point_exception: return "#XM/#XF";
        case interrupt_t::virtualization_exception: return "#VE";
        case interrupt_t::control_protection: return "#CP";
        case interrupt_t::hypervisor_injection: return "#HV";
        case interrupt_t::vmm_communication: return "#VC";
        case interrupt_t::security_exception: return "#SX";
        default: return "";
    }
}

}
