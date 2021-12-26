cmake_minimum_required(VERSION 3.21)
project(arch CXX ASM_NASM)

set(CMAKE_CXX_STANDARD 17)

add_library(arch
        src/intrinsics.cpp

        include/x86/cpuid.h
        include/x86/cr.h
        include/x86/msr.h

        include/x86/io.h src/x86/paging/paging.cpp include/x86/paging/bit32.h src/x86/paging/bit32.cpp src/x86/cpuid.cpp include/x86/segmentation.h src/x86/segmentation.cpp)

target_compile_options(arch PRIVATE -ffreestanding)
target_link_options(arch PRIVATE -nostdlib -nolibc -nodefaultlibs)
target_include_directories(arch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
