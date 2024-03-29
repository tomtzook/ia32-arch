#pragma once

#include "macros.h"


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

typedef unsigned long long size_t;
typedef long long ssize_t;

// even in 32 bit paging mode the address could sometimes be
// up to 40 bits long. In order to handle all situations, this
// type will always be big enough to hold all addresses.
typedef uint64_t physical_address_t;

typedef uintn_t linear_address_t;
