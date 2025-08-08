#pragma once

#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef uint32_t uintptr_t;

typedef uint32_t size_t;

#define true  1
#define false 0
#define NULL  ((void *) 0)