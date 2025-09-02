#include "hal.h"
#include "include/sbi.h"
#include "include/sys/task.h"
#include "lib/malloc.h"
#include "libc.h"



void kernel_main(void)
{
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
    heap_init((void *) &__heap_top, (size_t) &__heap_size);

    int i = 1;
    

    while (1) {
        __asm__ __volatile__("wfi");
    };
}

__attribute__((section(".text.boot"))) __attribute__((naked)) void boot(void)
{
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n"  // Set the stack pointer
        "j kernel_main\n"        // Jump to the kernel main function
        :
        : [stack_top] "r"(
            __stack_top)  // Pass the stack top address as %[stack_top]
    );
}
