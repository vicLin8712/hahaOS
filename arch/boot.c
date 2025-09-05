#include "type.h"


extern char __stack_top[];

void main(void);

#define MIE (1u << 11)
__attribute__((section(".text.boot"))) __attribute__((naked)) void _entry(void)
{
    __asm__ __volatile__(
        "mv sp, %0\n"  // Set the stack pointer

        "la t0, _isr\n"    /* Load _isr address*/
        "csrw mtvec, t0\n" /* Store _isr address to stvec */

        "li t0, 0x00000080\n"
        "csrw mie, t0\n"
        "csrs mstatus, 8\n"
        "call main\n"  // Jump to the kernel main function
        :
        : [stack_top] "r"(
            __stack_top)  // Pass the stack top address as %[stack_top]
    );
}

#define ISR_CONTEXT_SIZE 128

/* Low-level Interrupt Service Routine (ISR) trampoline.
 *
 * This is the common entry point for all traps. It performs a FULL context
 * save, creating a complete trap frame on the stack. This makes the C handler
 * robust, as it does not need to preserve any registers itself.
 */
__attribute__((naked, aligned(4))) void _isr(void)
{
    __asm__ __volatile__(
        /* Allocate stack frame for full context save */
        "addi   sp, sp, -%0\n"

        /* Save all general-purpose registers except x0 (zero) and x2 (sp).
         * This includes caller-saved and callee-saved registers.
         *
         * Stack Frame Layout (offsets from sp in bytes):
         *   0: ra,   4: gp,   8: tp,  12: t0,  16: t1,  20: t2
         *  24: s0,  28: s1,  32: a0,  36: a1,  40: a2,  44: a3
         *  48: a4,  52: a5,  56: a6,  60: a7,  64: s2,  68: s3
         *  72: s4,  76: s5,  80: s6,  84: s7,  88: s8,  92: s9
         *  96: s10, 100:s11, 104:t3, 108: t4, 112: t5, 116: t6
         * 120: mcause, 124: mepc
         */
        "sw  ra,   0*4(sp)\n"
        "sw  gp,   1*4(sp)\n"
        "sw  tp,   2*4(sp)\n"
        "sw  t0,   3*4(sp)\n"
        "sw  t1,   4*4(sp)\n"
        "sw  t2,   5*4(sp)\n"
        "sw  s0,   6*4(sp)\n"
        "sw  s1,   7*4(sp)\n"
        "sw  a0,   8*4(sp)\n"
        "sw  a1,   9*4(sp)\n"
        "sw  a2,  10*4(sp)\n"
        "sw  a3,  11*4(sp)\n"
        "sw  a4,  12*4(sp)\n"
        "sw  a5,  13*4(sp)\n"
        "sw  a6,  14*4(sp)\n"
        "sw  a7,  15*4(sp)\n"
        "sw  s2,  16*4(sp)\n"
        "sw  s3,  17*4(sp)\n"
        "sw  s4,  18*4(sp)\n"
        "sw  s5,  19*4(sp)\n"
        "sw  s6,  20*4(sp)\n"
        "sw  s7,  21*4(sp)\n"
        "sw  s8,  22*4(sp)\n"
        "sw  s9,  23*4(sp)\n"
        "sw  s10, 24*4(sp)\n"
        "sw  s11, 25*4(sp)\n"
        "sw  t3,  26*4(sp)\n"
        "sw  t4,  27*4(sp)\n"
        "sw  t5,  28*4(sp)\n"
        "sw  t6,  29*4(sp)\n"

        /* Save trap-related CSRs and prepare arguments for do_trap */
        "csrr   a0, mcause\n" /* Arg 1: cause */
        "csrr   a1, mepc\n"   /* Arg 2: epc */
        "sw     a0,  30*4(sp)\n"
        "sw     a1,  31*4(sp)\n"

        /* Call the high-level C trap handler */
        "call   do_trap\n"

        /* Restore context. mepc might have been modified by the handler */
        "lw     a1,  31*4(sp)\n"
        "csrw   mepc, a1\n"
        "lw  ra,   0*4(sp)\n"
        "lw  gp,   1*4(sp)\n"
        "lw  tp,   2*4(sp)\n"
        "lw  t0,   3*4(sp)\n"
        "lw  t1,   4*4(sp)\n"
        "lw  t2,   5*4(sp)\n"
        "lw  s0,   6*4(sp)\n"
        "lw  s1,   7*4(sp)\n"
        "lw  a0,   8*4(sp)\n"
        "lw  a1,   9*4(sp)\n"
        "lw  a2,  10*4(sp)\n"
        "lw  a3,  11*4(sp)\n"
        "lw  a4,  12*4(sp)\n"
        "lw  a5,  13*4(sp)\n"
        "lw  a6,  14*4(sp)\n"
        "lw  a7,  15*4(sp)\n"
        "lw  s2,  16*4(sp)\n"
        "lw  s3,  17*4(sp)\n"
        "lw  s4,  18*4(sp)\n"
        "lw  s5,  19*4(sp)\n"
        "lw  s6,  20*4(sp)\n"
        "lw  s7,  21*4(sp)\n"
        "lw  s8,  22*4(sp)\n"
        "lw  s9,  23*4(sp)\n"
        "lw  s10, 24*4(sp)\n"
        "lw  s11, 25*4(sp)\n"
        "lw  t3,  26*4(sp)\n"
        "lw  t4,  27*4(sp)\n"
        "lw  t5,  28*4(sp)\n"
        "lw  t6,  29*4(sp)\n"

        /* Deallocate stack frame */
        "addi   sp, sp, %0\n"

        /* Return from trap */
        "mret\n"
        : /* no outputs */
        : "i"(ISR_CONTEXT_SIZE)
        : "memory");
}