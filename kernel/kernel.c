#include "kernel.h"
#include "sbi/sbi.h"
#include "mm/mm.h"
#include "print/print.h"
#include "sections.h"




void kernel_main(void) {
    
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
	const char *s = "\n\nHello World!\n";

    for (int i = 0; s[i] != '\0'; i++) {
        putchar(s[i]);
    }

    char buffer[15];
    char *test = buffer;
    memcpy(test, s, 13);
    print("test result\n");
    print (test);
	

    for (;;) {
        __asm__ __volatile__("wfi");
    }
}


__attribute__((section(".text.boot")))
__attribute__((naked))
void boot(void) {
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n" // Set the stack pointer
        "j kernel_main\n"       // Jump to the kernel main function
        :
        : [stack_top] "r" (__stack_top) // Pass the stack top address as %[stack_top]
    );
}
