#pragma once

#include "types.h"


void memset(void* dest, uint8_t value, size_t size);
void* memcpy(void* dest, const void* src, size_t size);

size_t strlen(const char* s);
