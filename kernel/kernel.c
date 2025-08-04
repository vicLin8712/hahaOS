#include "kernel.h"
#include "mm/mm.h"
#include "print/print.h"
#include "sbi/sbi.h"
#include "sections.h"



void kernel_main(void)
{
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);

    char *s1 = "HELLO";
    char buf[10];
    char *s2 = buf;
    strcpy(s2, s1);
    print("copy str is %s", s2);

    for (;;) {
        __asm__ __volatile__("wfi");
    }
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
