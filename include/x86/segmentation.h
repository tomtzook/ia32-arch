#pragma once

#include "common.h"


namespace x86 {
namespace segmentation {

#pragma pack(push, 1)

// Segment Selectors [SDM 3 3.4.2 P95]
enum class table_type_t : uint16_t {
    GDT = 0,
    LDT = 1
};

struct selector_t {
    union {
        struct {
            uint16_t rpl : 2;
            table_type_t table : 1;
            uint16_t index: 13;
        } bits;
        uint16_t value;
    };
};
static_assert(sizeof(selector_t) == 2, "sizeof(segment_selector_t)");

struct cs_t : public selector_t {};
struct ds_t : public selector_t {};
struct gs_t : public selector_t {};
struct ss_t : public selector_t {};
struct es_t : public selector_t {};
struct fs_t : public selector_t {};

// Segment Descriptors [SDM 3 3.4.5 P98]

enum class type_t : uint64_t {
    DATA_READ_ONLY = 0b0000,
    DATA_READ_ONLY_ACCESSED = 0b0001,
    DATA_READ_WRITE = 0b0010,
    DATA_READ_WRITE_ACCESSED = 0b0011,
    DATA_READ_ONLY_EXPAND_DOWN = 0b0100,
    DATA_READ_ONLY_EXPAND_DOWN_ACCESSED = 0b0101,
    DATA_READ_WRITE_EXPAND_DOWN = 0b0110,
    DATA_READ_WRITE_EXPAND_DOWN_ACCESSED = 0b0111,
    CODE_EXECUTE_ONLY = 0b1000,
    CODE_EXECUTE_ONLY_ACCESSED = 0b1001,
    CODE_EXECUTE_READ = 0b1010,
    CODE_EXECUTE_READ_ACCESSED = 0b1011,
    CODE_EXECUTE_ONLY_CONFORMING = 0b1100,
    CODE_EXECUTE_ONLY_CONFORMING_ACCESSED = 0b1101,
    CODE_EXECUTE_READ_CONFORMING = 0b1110,
    CODE_EXECUTE_READ_CONFORMING_ACCESSED = 0b1111,

    SYSTEM_RESERVED0 = 0b0000,
    SYSTEM_BITS16_TSS_AVAILABLE = 0b0001,
    SYSTEM_LDT = 0b0010,
    SYSTEM_BITS16_TSS_BUSY = 0b0011,
    SYSTEM_BITS16_CALL_GATE = 0b0100,
    SYSTEM_TASK_GATE = 0b0101,
    SYSTEM_BITS16_INTERRUPT_GATE = 0b0110,
    SYSTEM_BITS16_TRAP_GATE = 0b0111,
    SYSTEM_RESERVED1 = 0b1000,
    SYSTEM_BITS32_TSS_AVAILABLE = 0b1001,
    SYSTEM_RESERVED2 = 0b1010,
    SYSTEM_BITS32_TSS_BUSY = 0b1011,
    SYSTEM_BITS32_CALL_GATE = 0b1100,
    SYSTEM_RESERVED3 = 0b1101,
    SYSTEM_BITS32_INTERRUPT = 0b1110,
    SYSTEM_BITS32_TRAP_GATE = 0b1111
};

enum class descriptor_type_t {
    SYSTEM = 0,
    CODE_OR_DATA = 1
};

enum class granularity_t : uint64_t {
    BYTE = 0,
    PAGE = 1
};

// In 64-bit mode, the Base and Limit values are ignored,
// each descriptor covers the entire linear address space
// regardless of what they are set to.
struct descriptor_t {
    union {
        struct {
            uint64_t limit_low : 16;
            uint64_t base_address_low : 16;
            uint64_t base_address_middle : 8;
            uint64_t type : 4;
            descriptor_type_t s : 1;
            uint64_t dpl : 2;
            uint64_t present : 1;
            uint64_t limit_high : 4;
            uint64_t available : 1;
            uint64_t long_mode : 1;
            uint64_t default_db : 1;
            granularity_t granularity : 1;
            uint64_t base_address_high : 8;
        } bits;
        uint64_t raw;
    };

    linear_address_t base_address() const noexcept;
    void base_address(linear_address_t address) noexcept;

    size_t limit() const noexcept;
    void limit(size_t limit) noexcept;
};
static_assert(sizeof(descriptor_t) == 8, "sizeof(segment_descriptor_t)");

struct table_register_t {
    uint16_t limit;
    uint32_t base_address;
};
static_assert(sizeof(table_register_t) == 6, "sizeof(table_register_t)");

struct gdtr_t : public table_register_t {};
struct ldtr_t : public table_register_t {};

#pragma pack(pop)

class table_t {
public:
    table_t(table_register_t table_register) noexcept;

    const void* base_address() const noexcept;
    void* base_address() noexcept;

    size_t limit() const noexcept;

    const descriptor_t& operator[](size_t index) const noexcept;
    descriptor_t& operator[](size_t index) noexcept;

    const descriptor_t& operator[](const selector_t& selector) const noexcept;
    descriptor_t& operator[](const selector_t& selector) noexcept;

private:
    table_register_t m_table_register;
};

}

allow_struct_read_write(segmentation::gdtr_t);

template<>
inline segmentation::gdtr_t read() noexcept {
    segmentation::gdtr_t gdtr{};
    asm volatile("sgdt %0" : "=m"(gdtr));
    return gdtr;
}

template<>
inline void write(const segmentation::gdtr_t& t) noexcept {
    asm volatile("lgdt %0" : : "m"(t));
}

allow_struct_read_write(segmentation::ldtr_t);

template<>
inline segmentation::ldtr_t read() noexcept {
    segmentation::ldtr_t ldtr{};
    asm volatile("sldt %0" : "=m"(ldtr));
    return ldtr;
}

template<>
inline void write(const segmentation::ldtr_t& t) noexcept {
    asm volatile("lldt %0" : : "m"(t));
}

allow_struct_read_write(segmentation::cs_t);

template<>
inline segmentation::cs_t read() noexcept {
    segmentation::cs_t selector{};
    asm volatile("mov %%cs, %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segmentation::cs_t& t) noexcept {
    asm volatile("mov %0, %%cs" : : "rm"(t.value));
}

allow_struct_read_write(segmentation::ds_t);

template<>
inline segmentation::ds_t read() noexcept {
    segmentation::ds_t selector{};
    asm volatile("mov %%ds, %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segmentation::ds_t& t) noexcept {
    asm volatile("mov %0, %%ds" : : "rm"(t.value));
}

allow_struct_read_write(segmentation::gs_t);

template<>
inline segmentation::gs_t read() noexcept {
    segmentation::gs_t selector{};
    asm volatile("mov %%gs, %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segmentation::gs_t& t) noexcept {
    asm volatile("mov %0, %%gs" : : "rm"(t.value));
}

allow_struct_read_write(segmentation::ss_t);

template<>
inline segmentation::ss_t read() noexcept {
    segmentation::ss_t selector{};
    asm volatile("mov %%ss, %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segmentation::ss_t& t) noexcept {
    asm volatile("mov %0, %%ss" : : "rm"(t.value));
}

allow_struct_read_write(segmentation::es_t);

template<>
inline segmentation::es_t read() noexcept {
    segmentation::es_t selector{};
    asm volatile("mov %%es, %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segmentation::es_t& t) noexcept {
    asm volatile("mov %0, %%es" : : "rm"(t.value));
}

allow_struct_read_write(segmentation::fs_t);

template<>
inline segmentation::fs_t read() noexcept {
    segmentation::fs_t selector{};
    asm volatile("mov %%fs, %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segmentation::fs_t& t) noexcept {
    asm volatile("mov %0, %%fs" : : "rm"(t.value));
}

}
