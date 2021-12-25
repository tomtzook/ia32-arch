#pragma once

#include "types.h"

int memcmp(const void* s1, const void* s2, size_t size);
void memset(void* dest, uint8_t value, size_t size);
void* memcpy(void* dest, const void* src, size_t size);

size_t strlen(const char* s);
int strcmp(const char* s1, const char* s2);
