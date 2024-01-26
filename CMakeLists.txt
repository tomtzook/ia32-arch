cmake_minimum_required(VERSION 3.23)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(arch
        src/x86/paging/paging.cpp
        src/x86/paging/bit32.cpp
        src/x86/cpuid.cpp
        src/x86/segments.cpp
        src/x86/interrupts.cpp
        src/x86/paging/pae.cpp
        src/x86/paging/ia32e.cpp
        src/x86/apic.cpp
        src/intrinsics.cpp
        src/x86/vmx/vmx.cpp
        src/x86/vmx/ept.cpp

        # adding headers just for IDE to work with them better
        include/meta.h
        include/x86/cpuid.h
        include/x86/cr.h
        include/x86/msr.h
        include/x86/io.h
        include/x86/paging/bit32.h
        include/x86/segments.h
        include/x86/interrupts.h
        include/x86/paging/pae.h
        include/x86/paging/ia32e.h
        include/x86/apic.h
        include/x86/vmx/vmcs.h
        include/x86/vmx/vmx.h
        include/x86/vmx/error.h
        include/x86/vmx/ept.h
        include/x86/vmx/controls.h include/x86/vmx/segments.h)

target_compile_options(arch PRIVATE -ffreestanding)
target_link_options(arch PRIVATE -nostdlib -nolibc -nodefaultlibs)
target_include_directories(arch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
