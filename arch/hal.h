#pragma once
#include "type.h"
/* Define buffer for task switching.
 * Memory layouts (14 x 32-bit words)
 * [0-11]: s0-s11 (callee-saved registers)
 * [12]: sp (stack pointer)
 * [13]: ra (return address)
 */
typedef uint32_t jmp_buf[14];

/* Save execution context only*/
int32_t setjmp(jmp_buf env);
/* Restore execution context only*/
int32_t longjmp(jmp_buf env, int32_t val);