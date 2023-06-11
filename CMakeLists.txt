cmake_minimum_required(VERSION 3.23)

add_library(arch
        src/x86/paging/paging.cpp
        src/x86/paging/bit32.cpp
        src/x86/cpuid.cpp
        src/x86/segmentation.cpp
        src/x86/interrupts.cpp
        src/x86/paging/pae.cpp
        src/x86/paging/ia32e.cpp
        src/x86/apic.cpp
        src/intrinsics.cpp
        src/x86/vmx/vmx.cpp

        # adding headers just for IDE to work with them better
        include/x86/cpuid.h
        include/x86/cr.h
        include/x86/msr.h
        include/x86/io.h
        include/x86/paging/bit32.h
        include/x86/segmentation.h
        include/x86/interrupts.h
        include/x86/paging/pae.h
        include/x86/paging/ia32e.h
        include/x86/apic.h
        include/x86/vmx/vmcs.h
        include/x86/vmx/vmx.h
)

target_compile_options(arch PRIVATE -ffreestanding)
target_link_options(arch PRIVATE -nostdlib -nolibc -nodefaultlibs)
target_include_directories(arch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
