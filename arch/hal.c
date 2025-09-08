#include "hal.h"
#include "libc.h"
#include "sys/task.h"
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

/* NS16550A UART0 - Memory-mapped registers for the QEMU 'virt' machine's serial
 * port.
 */

#define NS16550A_UART0_BASE 0x10000000U
#define NS16550A_UART0_REG(off) \
    (*(volatile uint8_t *) (NS16550A_UART0_BASE + (off)))

/* UART register offsets */
#define NS16550A_THR 0x00 /* Transmission holding register */
#define NS16550A_RBR 0x00 /* Reception buffer register */
#define NS16550A_DLH 0x00 /* Divisor latch high (DLAB = 1) */
#define NS16550A_DLL 0x01 /* Divisor latch low (DLAB = 1) */
#define NS16550A_LCR 0x03 /* Line Control Register */
#define NS16550A_LSR 0x05 /* Line Status Register */

/* Line Status Register bits */
#define NS16550A_LSR_DR 0x01 /* Data Ready: byte received */
/* Transmit Holding Register Empty: ready to send */
#define NS16550A_LSR_THRE 0x20

/* Line Control Register bits */
#define NS16550A_LCR_8BIT 0x03 /* 8-bit chars, no parity, 1 stop bit (8N1) */
#define NS16550A_LCR_DLAB 0x80 /* Divisor Latch Access Bit */

/* CLINT registers
 * only mtime and mtimecmp used here
 */
#define CLINT_BASE 0x02000000UL

#define MTIMECMP (*(volatile uint64_t *) (CLINT_BASE + 0x4000u))
#define MTIMECMP_L (*(volatile uint32_t *) (CLINT_BASE + 0x4000u))
#define MTIMECMP_H (*(volatile uint32_t *) (CLINT_BASE + 0x4004u))

#define MTIME (*(volatile uint64_t *) (CLINT_BASE + 0xBFF8u))
#define MTIME_L (*(volatile uint32_t *) (CLINT_BASE + 0xBFF8u))
#define MTIME_H (*(volatile uint32_t *) (CLINT_BASE + 0xBFFCu))

/* Low-level I/O and delay */
int putchar(int value)
{
    volatile uint32_t timeout = 0x100000;
    while (!(NS16550A_UART0_REG(NS16550A_LSR) & NS16550A_LSR_THRE)) {
        if (unlikely(--timeout == 0))
            return 0;
    }
    NS16550A_UART0_REG(NS16550A_THR) = (uint8_t) value;
    return value;
}

/* Initialize UART */
void uart_init(uint32_t baud)
{
    uint32_t divisor = F_CPU / (16 * baud);
    if (unlikely(!divisor))
        divisor = 1; /* Ensure non-zero divisor */

    /* Set DLAB to access divisor registers */
    NS16550A_UART0_REG(NS16550A_LCR) = NS16550A_LCR_DLAB;
    NS16550A_UART0_REG(NS16550A_DLH) = (divisor >> 8) & 0xff;
    NS16550A_UART0_REG(NS16550A_DLL) = divisor & 0xff;
    /* Clear DLAB and set line control to 8N1 mode */
    NS16550A_UART0_REG(NS16550A_LCR) = NS16550A_LCR_8BIT;
}


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


int hal_context_save(jmp_buf env)
{
    if ((unlikely(!env)))
        return -1;

    __asm__ __volatile__(
        /* Store callee saved registers and ra sp*/
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

        /* Save mstatus with interrupt state reconstruction. During timer
         * interrupts, mstatus.MIE is cleared, so we reconstruct the pre-trap
         * state from MPIE for consistent interrupt context preservation.
         */
        "csrr t0, mstatus\n" /* Read current mstatus (MIE=0 in trap) */
        "andi t1, t0, ~8\n"  /* Clear MIE bit first */
        "srli t2, t0, 4\n"   /* Get MPIE bit to position 3 */
        "andi t2, t2, 8\n"   /* Isolate bit 3 */
        "or   t1, t1, t2\n"  /* Combine cleared MIE with reconstructed bit */
        "sw   t1, 14*4(%0)\n"

        :
        : "r"(env)
        : "t0", "t1", "t2", "memory", "a0");
    return 0;
}

__attribute__((noreturn)) void hal_context_restore(jmp_buf env, int32_t val)
{
    if (unlikely(!env))
        hal_panic(); /* Cannot proceed with invalid context */

    if (val == 0)
        val = 1; /* Must return a non-zero value after restore */

    __asm__ __volatile__(
        /* Restore all registers from the provided 'jmp_buf' */
        "lw  s0,   0*4(%0)\n"
        "lw  s1,   1*4(%0)\n"
        "lw  s2,   2*4(%0)\n"
        "lw  s3,   3*4(%0)\n"
        "lw  s4,   4*4(%0)\n"
        "lw  s5,   5*4(%0)\n"
        "lw  s6,   6*4(%0)\n"
        "lw  s7,   7*4(%0)\n"
        "lw  s8,   8*4(%0)\n"
        "lw  s9,   9*4(%0)\n"
        "lw  s10, 10*4(%0)\n"
        "lw  s11, 11*4(%0)\n"
        "lw  sp,  12*4(%0)\n"
        "lw  ra,  13*4(%0)\n"
        "lw  t0,  14*4(%0)\n"
        "csrw  mstatus,  t0\n"
        /* Set the return value (in 'a0') */
        "mv  a0,  %1\n"
           
        /* "Return" to the restored 'ra', effectively jumping to new context */
        "ret\n"
        :
        : "r"(env), "r"(val)
        : "memory");

    __builtin_unreachable(); /* Tell compiler this point is never reached */
}

static void __attribute__((naked, used)) __dispatch_init(void)
{
    __asm__ __volatile__(
        "lw  s0,   0*4(a0)\n"
        "lw  s1,   1*4(a0)\n"
        "lw  s2,   2*4(a0)\n"
        "lw  s3,   3*4(a0)\n"
        "lw  s4,   4*4(a0)\n"
        "lw  s5,   5*4(a0)\n"
        "lw  s6,   6*4(a0)\n"
        "lw  s7,   7*4(a0)\n"
        "lw  s8,   8*4(a0)\n"
        "lw  s9,   9*4(a0)\n"
        "lw  s10, 10*4(a0)\n"
        "lw  s11, 11*4(a0)\n"
        "lw  sp,  12*4(a0)\n"
        "lw  ra,  13*4(a0)\n"
        "ret\n"); /* Jump to the task's entry point */
}
__attribute__((noreturn))  void hal_scheduler_init(jmp_buf env) {
    uint64_t current = ((uint64_t) MTIME_H << 32) | MTIME_L;
    uint64_t newtime = current + F_CPU / 1000; /* Timer interrupt per ms */

    MTIMECMP_H = 0xFFFFFFFFu;
    MTIMECMP_L = (uint32_t) newtime;
    MTIMECMP_H = (uint32_t)(newtime >> 32);
    
    ei();
    write_csr(mie, read_csr(mie) | 1U << 7);
    printf("GGG");

    __asm__ __volatile__(
        "mv  a0, %0\n"           /* Move @env (the task's context) into 'a0' */
        "call __dispatch_init\n" /* Call the low-level restore routine */
        :
        : "r"(env)
        : "a0", "memory");
    __builtin_unreachable();
}

/* Trap control */
void do_trap()
{
    /* Read time & reset new time */
    uint64_t current = ((uint64_t) MTIME_H << 32) | MTIME_L;
    uint64_t newtime = current + F_CPU/10000 ; /* Timer interrupt per ms */

    MTIMECMP_H = 0xFFFFFFFFu;
    MTIMECMP_L = (uint32_t) newtime;
    MTIMECMP_H = (uint32_t) (newtime >> 32);
    scheduler();
}

void hal_panic(void)
{
    while (1)
        __asm__ __volatile__("wfi");
}
