cmake_minimum_required(VERSION 3.21)
project(arch CXX ASM_NASM)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
add_library(arch
        src/intrinsics.cpp

        include/x86/cpuid.h
        include/x86/cr.h
        include/x86/msr.h

        include/x86/io.h)

target_compile_options(arch PRIVATE -ffreestanding)
target_link_options(arch PRIVATE -nostdlib -nolibc -nodefaultlibs)
