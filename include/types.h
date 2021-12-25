#pragma once

#include "macros.h"


#ifdef X86_64
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;
typedef int64_t intn_t;
typedef int64_t intptr_t;
typedef long int intmax_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint64_t uintn_t;
typedef uint64_t uintptr_t;
typedef unsigned long int uintmax_t;
#else
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed long int32_t;
typedef signed long long int64_t;
typedef int32_t intn_t;
typedef int32_t intptr_t;
typedef long long int intmax_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t uintn_t;
typedef uint32_t uintptr_t;
typedef unsigned long long int uintmax_t;
#endif

typedef unsigned long int size_t;
typedef long int ssize_t;

typedef uintn_t physical_address_t;
