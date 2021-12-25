cmake_minimum_required(VERSION 3.21)
project(arch CXX ASM_NASM)

set(CMAKE_CXX_STANDARD 17)

add_library(arch
        src/intrinsics.cpp

        include/x86/cpuid.h
        include/x86/cr.h
        include/x86/msr.h

        include/x86/io.h)

target_compile_options(arch PRIVATE -ffreestanding)
target_link_options(arch PRIVATE -nostdlib -nolibc -nodefaultlibs)
target_include_directories(arch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
