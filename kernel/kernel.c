#include "include/sys/task.h"
#include "include/libc.h"
#include "include/sbi.h"
#include "include/sections.h"



void kernel_main(void) {
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
    
    struct process *procA = create_process((uint32_t) &task_A);
    struct process *procB = create_process((uint32_t) &task_B);    
    struct process *new_proc = scheduler();
    switch_context(&procB->sp, &new_proc->sp);

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
