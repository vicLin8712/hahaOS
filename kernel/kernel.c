#include "include/libc.h"
#include "include/sbi.h"
#include "include/sections.h"
#include "include/sys/task.h"

void task_A()
{
    printf("task_A executed\n");
    yield();
}
void task_B()
{
    printf("task_B executed\n");
    yield();
}

void task_C()
{
    printf("task_C executed\n");
    yield();
}

void task_D()
{
    printf("task_D executed\n");
    task_A();
}



void kernel_main(void)
{
    memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);
    
    while (1) {
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
