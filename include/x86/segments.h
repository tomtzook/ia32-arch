#pragma once

#include "x86/common.h"


namespace x86 {
namespace segments {

#pragma pack(push, 1)

// Segment Selectors [SDM 3 3.4.2 P95]
enum class table_type_t : uint16_t {
    gdt = 0,
    ldt = 1
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

template<typename _t>
struct is_selector_t : public meta::false_type {};
#define define_selector(name) \
    struct name : public selector_t {}; \
    template<> struct is_selector_t<name> : public meta::true_type {};

define_selector(cs_t);
define_selector(ds_t);
define_selector(gs_t);
define_selector(ss_t);
define_selector(es_t);
define_selector(fs_t);
define_selector(tr_t);

// Segment Descriptors [SDM 3 3.4.5 P98]

enum class type_t : uint64_t {
    data_read_only = 0b0000,
    data_read_only_accessed = 0b0001,
    data_read_write = 0b0010,
    data_read_write_accessed = 0b0011,
    data_read_only_expand_down = 0b0100,
    data_read_only_expand_down_accessed = 0b0101,
    data_read_write_expand_down = 0b0110,
    data_read_write_expand_down_accessed = 0b0111,
    code_execute_only = 0b1000,
    code_execute_only_accessed = 0b1001,
    code_execute_read = 0b1010,
    code_execute_read_accessed = 0b1011,
    code_execute_only_confirming = 0b1100,
    code_execute_only_confirming_accessed = 0b1101,
    code_execute_read_confirming = 0b1110,
    code_execute_read_confirming_accessed = 0b1111,

    system_reserved0 = 0b0000,
    system_bits16_tss_available = 0b0001,
    system_ldt = 0b0010,
    system_bits16_tss_busy = 0b0011,
    system_bits16_call_gate = 0b0100,
    system_task_gate = 0b0101,
    system_bits16_interrupt_gate = 0b0110,
    system_bits16_trap_gate = 0b0111,
    system_reserved1 = 0b1000,
    system_bits32_tss_available = 0b1001,
    system_reserved2 = 0b1010,
    system_bits32_tss_busy = 0b1011,
    system_bits32_call_gate = 0b1100,
    system_reserved3 = 0b1101,
    system_bits32_interrupt = 0b1110,
    system_bits32_trap_gate = 0b1111
};

enum class system64_type_t : uint64_t {
    system_ldt = 0b0010,
    system_bits64_tss_available = 0b1001,
    system_bits64_tss_busy = 0b1011,
    system_bits64_call_gate = 0b1100,
    system_bits64_interrupt = 0b1110,
    system_bits64_trap_gate = 0b1111
};

enum class descriptor_type_t : uint64_t {
    system = 0,
    code_or_data = 1
};

enum class granularity_t : uint64_t {
    byte = 0,
    page = 1
};

// [SDM 3 3.4.5 "Figure 3-8"]
struct descriptor_t {
    union {
        struct {
            uint64_t limit_low : 16;
            uint64_t base_address_low : 16;
            uint64_t base_address_middle : 8;
            type_t type : 4;
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
static_assert(sizeof(descriptor_t) == 8, "sizeof(descriptor_t)");

// [SDM 3 7.2.3 "Figure 7-4"]
struct descriptor64_t {
    descriptor_t base;
    union {
        struct {
            uint64_t base_address_upper : 32;
            uint64_t ignored0 : 32;
        } bits;
        uint64_t raw;
    };

    linear_address_t base_address() const noexcept;
    void base_address(linear_address_t address) noexcept;

    size_t limit() const noexcept;
    void limit(size_t limit) noexcept;
};
static_assert(sizeof(descriptor64_t) == 16, "sizeof(descriptor64_t)");

struct table_register_t {
    uint16_t limit;
    uint64_t base_address;
};
static_assert(sizeof(table_register_t) == 10, "sizeof(table_register_t)");

struct gdtr_t : public table_register_t {};
struct ldtr_t : public table_register_t {};

// [SDM 3 7.7 "Figure 7-11"]
struct tss64_t {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist[7]; // 1-7
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t io_map_base;
};
static_assert(sizeof(tss64_t) == 104, "sizeof(tss64_t)");

#pragma pack(pop)

class table_t {
public:
    table_t(table_register_t table_register) noexcept;

    const void* base_address() const noexcept;
    void* base_address() noexcept;

    size_t limit() const noexcept;
    size_t count() const noexcept;

    const descriptor_t& operator[](size_t index) const noexcept;
    descriptor_t& operator[](size_t index) noexcept;

    const descriptor_t& operator[](const selector_t& selector) const noexcept;
    descriptor_t& operator[](const selector_t& selector) noexcept;

    template<
            typename _t,
            typename meta::enable_if<
                    is_selector_t<_t>::value,
                    bool>::type = 0
    >
    const descriptor_t& segment() const noexcept {
        selector_t selector = read<_t>();
        return this->operator[](selector);
    }

    template<
            typename _t,
            typename meta::enable_if<
                    is_selector_t<_t>::value,
                    bool>::type = 0
    >
    descriptor_t& segment() noexcept {
        selector_t selector = read<_t>();
        return this->operator[](selector);
    }

private:
    table_register_t m_table_register;
};

}

allow_struct_read_write(segments::gdtr_t);

template<>
inline segments::gdtr_t read() noexcept {
    segments::gdtr_t gdtr{};
    asm volatile("sgdt %0" : "=m"(gdtr));
    return gdtr;
}

template<>
inline void write(const segments::gdtr_t& t) noexcept {
    asm volatile("lgdt %0" : : "m"(t));
}

allow_struct_read_write(segments::ldtr_t);

template<>
inline segments::ldtr_t read() noexcept {
    segments::ldtr_t ldtr{};
    asm volatile("sldt %0" : "=m"(ldtr));
    return ldtr;
}

template<>
inline void write(const segments::ldtr_t& t) noexcept {
    asm volatile("lldt %0" : : "m"(t));
}

allow_struct_read_write(segments::cs_t);

template<>
inline segments::cs_t read() noexcept {
    segments::cs_t selector{};
    asm volatile("mov %%cs, %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segments::cs_t& t) noexcept {
    asm volatile("mov %0, %%cs" : : "rm"(t.value));
}

allow_struct_read_write(segments::ds_t);

template<>
inline segments::ds_t read() noexcept {
    segments::ds_t selector{};
    asm volatile("mov %%ds, %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segments::ds_t& t) noexcept {
    asm volatile("mov %0, %%ds" : : "rm"(t.value));
}

allow_struct_read_write(segments::gs_t);

template<>
inline segments::gs_t read() noexcept {
    segments::gs_t selector{};
    asm volatile("mov %%gs, %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segments::gs_t& t) noexcept {
    asm volatile("mov %0, %%gs" : : "rm"(t.value));
}

allow_struct_read_write(segments::ss_t);

template<>
inline segments::ss_t read() noexcept {
    segments::ss_t selector{};
    asm volatile("mov %%ss, %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segments::ss_t& t) noexcept {
    asm volatile("mov %0, %%ss" : : "rm"(t.value));
}

allow_struct_read_write(segments::es_t);

template<>
inline segments::es_t read() noexcept {
    segments::es_t selector{};
    asm volatile("mov %%es, %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segments::es_t& t) noexcept {
    asm volatile("mov %0, %%es" : : "rm"(t.value));
}

allow_struct_read_write(segments::fs_t);

template<>
inline segments::fs_t read() noexcept {
    segments::fs_t selector{};
    asm volatile("mov %%fs, %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segments::fs_t& t) noexcept {
    asm volatile("mov %0, %%fs" : : "rm"(t.value));
}

allow_struct_read_write(segments::tr_t);

template<>
inline segments::tr_t read() noexcept {
    segments::tr_t selector{};
    asm volatile("str %0" : "=rm"(selector.value));
    return selector;
}

template<>
inline void write(const segments::tr_t& t) noexcept {
    asm volatile("ltr %0" : : "rm"(t.value));
}

}
