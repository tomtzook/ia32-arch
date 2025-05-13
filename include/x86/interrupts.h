#pragma once

#include "x86/common.h"


namespace x86 {
namespace interrupts {

#pragma pack(push, 1)

// [SDM 3 6.3.1 P188 "Table 6-1"]

enum class interrupt_t {
    divide_error = 0,
    debug_exception = 1,
    nmi = 2,
    breakpoint = 3,
    overflow = 4,
    bound_range_exceeded = 5,
    invalid_opcode = 6,
    device_not_available = 7,
    double_fault = 8,
    coprocessor_segment_overrun = 9,
    invalid_tss = 10,
    segment_not_present = 11,
    stack_segment_fault = 12,
    general_protection = 13,
    page_fault = 14,
    fpu_floating_point_error = 16,
    alignment_check = 17,
    machine_check = 18,
    simd_floating_point_exception = 19,
    virtualization_exception = 20,
};

// IDT Descriptors [SDM 3 6.11 P196]

enum class gate_type_t : uint64_t {
    task = 0b0101,
    interrupt_16 = 0b0110,
    trap_16 = 0b0111,
    interrupt_32 = 0b1110,
    trap_32 = 0b1111,
};

struct descriptor_t {
    union {
        struct {
            uint64_t offset_low : 16;
            uint64_t segment_selector : 16;
            uint64_t reserved0 : 8;
            gate_type_t type : 4;
            uint64_t reserved1 : 1;
            uint64_t dpl : 2;
            uint64_t present : 1;
            uint64_t offset_high : 16;
        } bits;
        uint64_t raw;
    };

    uint32_t address() const;
    void address(uint32_t address);
};
static_assert(sizeof(descriptor_t) == 8, "sizeof(descriptor_t)");

struct descriptor64_t {
    union {
        struct {
            uint64_t offset_low : 16;
            uint64_t segment_selector : 16;
            uint64_t ist : 3;
            uint64_t reserved0 : 5;
            gate_type_t type : 4;
            uint64_t reserved1 : 1;
            uint64_t dpl : 2;
            uint64_t present : 1;
            uint64_t offset_high : 16;
        } bits;
        uint64_t raw;
    } low;
    union {
        struct {
            uint64_t offset_upper : 32;
            uint64_t ignored0 : 32;
        } bits;
        uint64_t raw;
    } high;

    uint64_t address() const;
    void address(uint64_t address);
};
static_assert(sizeof(descriptor64_t) == 16, "sizeof(descriptor_t)");

// Interrupt Descriptor Table [SDM 3 6.10 P195]

struct idtr_t {
    uint16_t limit;
    uint32_t base_address;
};
static_assert(sizeof(idtr_t) == 6, "sizeof(idtr_t)");

#pragma pack(pop)

class table64_t {
public:
    table64_t(idtr_t idtr);

    const void* base_address() const;
    void* base_address();

    size_t limit() const;
    size_t count() const;

    const descriptor64_t& operator[](size_t index) const;
    descriptor64_t& operator[](size_t index);

    const descriptor64_t& operator[](interrupt_t interrupt) const;
    descriptor64_t& operator[](interrupt_t interrupt);

private:
    idtr_t m_idtr;
};

}

allow_struct_read_write(interrupts::idtr_t);

template<>
inline interrupts::idtr_t read() {
    interrupts::idtr_t idtr{};
    asm volatile("sidt %0" : "=m"(idtr));
    return idtr;
}

template<>
inline void write(const interrupts::idtr_t& t) {
    asm volatile("lidt %0" : : "m"(t));
}

}
