cmake_minimum_required(VERSION 3.21)
project(arch CXX ASM_NASM)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
add_library(arch
        src/intrinsics.cpp src/x86/cpuid.cpp src/x86/cr.cpp)
target_compile_options(arch PRIVATE -ffreestanding)
target_link_options(arch PRIVATE -nostdlib -nolibc -nodefaultlibs)
