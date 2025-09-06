#pragma once
#include "type.h"

extern char __bss[];
extern char __bss_end[];

extern uint32_t __heap_top;
extern uint32_t __heap_end;
extern uint32_t __heap_size;

/* Define basic operation */
#define ALIGN4(x) (((x + 3u) >> 2) << 2)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define likely(x) __builtin_expect(!!(x), 1)

#define F_CPU 10000000

#define read_csr(reg)                                 \
    ({                                                \
        uint32_t __tmp;                               \
        __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
        __tmp;                                        \
    })
#define write_csr(reg, val) \
    ({ __asm__ __volatile__(" csrw " #reg ", %0" ::"r"(val)); })

static inline void hal_interrupt_set(int32_t enable)
{
    int32_t mstatus_reg = read_csr(mstatus), mie_reg = read_csr(mie);    /* Read mstatus */
    mstatus_reg &= ~8;                          /* Clear MIE */
    mie_reg &= ~0x80;
    mstatus_reg |= (enable & 1) << 3;           /* Left shift enable bit*/
    mie_reg |= (enable & 1) << 7;           /* Left shift enable bit*/
    write_csr(mstatus, mstatus_reg);
    write_csr(mie,mie_reg);
}

#define ei() hal_interrupt_set(1)
#define di() hal_interrupt_set(0)

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

/* HAL context switching routines for complete context management */
int32_t hal_context_save(jmp_buf env);
void hal_context_restore(jmp_buf env, int32_t val);

int putchar(int);
void uart_init(uint32_t);
void hal_panic(void);
void do_trap(void);
