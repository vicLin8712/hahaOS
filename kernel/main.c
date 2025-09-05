#include "hal.h"
#include "include/sys/task.h"
#include "lib/malloc.h"
#include "libc.h"
#include "sys/task.h"

void task_A(void)
{
    printf("TASK_A Triggered \n");
}

void task_B(void)
{
    printf("TASK_A Triggered \n");
}

void task_C(void)
{
    printf("TASK_A Triggered \n");
}

void main(void)
{
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
    heap_init((void *) &__heap_top, (size_t) &__heap_size);
    uart_init(115200);

    printf("\n\n\n\nHI,QEMU\n\n\n");
    printf("TASK CREATING....\n");

    create_task((uint32_t) task_A);
    create_task((uint32_t) task_B);
    create_task((uint32_t) task_C);
    do_trap();
    int i = 0;

    while (1) {
        __asm__ __volatile__("wfi");
        printf("trapped %d times 87 \n", i++);
    };
}
