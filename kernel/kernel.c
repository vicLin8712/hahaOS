#include "include/sys/task.h"
#include "include/libc.h"
#include "include/sbi.h"
#include "include/sections.h"

void task_A(){
    printf("task_A executed");
}
void task_B(){
    printf("task_B executed");
    while(1);
}

void kernel_main(void) {
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);

    create_process((uint32_t) &task_A); 
    create_process((uint32_t) &task_B); 
    
    

    while (1) {
        schedule();
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
