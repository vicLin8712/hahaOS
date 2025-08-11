#include "hal.h"
#include "type.h"


/* Define the registers*/
#define CONTEXT_S0 0   /* s0 (x8)  - Callee-saved register */
#define CONTEXT_S1 1   /* s1 (x9)  - Callee-saved register */
#define CONTEXT_S2 2   /* s2 (x18) - Callee-saved register */
#define CONTEXT_S3 3   /* s3 (x19) - Callee-saved register */
#define CONTEXT_S4 4   /* s4 (x20) - Callee-saved register */
#define CONTEXT_S5 5   /* s5 (x21) - Callee-saved register */
#define CONTEXT_S6 6   /* s6 (x22) - Callee-saved register */
#define CONTEXT_S7 7   /* s7 (x23) - Callee-saved register */
#define CONTEXT_S8 8   /* s8 (x24) - Callee-saved register */
#define CONTEXT_S9 9   /* s9 (x25) - Callee-saved register */
#define CONTEXT_S10 10 /* s10(x26) - Callee-saved register */
#define CONTEXT_S11 11 /* s11(x27) - Callee-saved register */
#define CONTEXT_SP 12  /* sp (x2)  - Stack Pointer */

int32_t setjmp(jmp_buf env)
{
    __asm__ __volatile__(
        "sw s0, 0*4(%0)\n"
        "sw s1, 1*4(%0)\n"
        "sw s2, 2*4(%0)\n"
        "sw s3, 3*4(%0)\n"
        "sw s4, 4*4(%0)\n"
        "sw s5, 5*4(%0)\n"
        "sw s6, 6*4(%0)\n"
        "sw s7, 7*4(%0)\n"
        "sw s8, 8*4(%0)\n"
        "sw s9,9*4(%0)\n"
        "sw s10, 10*4(%0)\n"
        "sw s11, 11*4(%0)\n"
        "sw sp, 12*4(%0)\n"
        "sw ra, 13*4(%0)\n"
        "li a0, 0" /* Make sure return value will be 0 */
        :
        : "r"(env) /* Put all stored value into this jmp_buf */
        : "memory", "a0");
    return 0;
}

__attribute__((noreturn)) int32_t longjmp(jmp_buf env, int32_t val)
{
    if (val == 0)
        val = 1;
    __asm__ __volatile__(
        "lw s0, 0*4(%0)\n"
        "lw s1, 1*4(%0)\n"
        "lw s2, 2*4(%0)\n"
        "lw s3, 3*4(%0)\n"
        "lw s4, 4*4(%0)\n"
        "lw s5, 5*4(%0)\n"
        "lw s6, 6*4(%0)\n"
        "lw s7, 7*4(%0)\n"
        "lw s8, 8*4(%0)\n"
        "lw s9, 9*4(%0)\n"
        "lw s10, 10*4(%0)\n"
        "lw s11, 11*4(%0)\n"
        "lw sp, 12*4(%0)\n"
        "lw ra, 13*4(%0)\n"
        "mv  a0,  %1\n"
        /* "Return" to the restored 'ra', effectively jumping to new context */
        "ret\n"
        :
    : "r"(env), "r"(val)
        : "memory");

    __builtin_unreachable();
}

void hal_panic(void)
{
    while (1)
        __asm__ __volatile__("wfi");
}