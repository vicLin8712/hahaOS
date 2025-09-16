#include "hal.h"
#include "include/sys/task.h"
#include "lib/malloc.h"
#include "libc.h"
#include "sys/error.h"
#include "sys/task.h"



void task_B(void)
{
    for (int i = 0; i < 10000000; i++) {
        printf("TASK_B Triggered %d times\n", i);
        __asm__ __volatile__("wfi");
    }
}

void task_A(void)
{
    for (int i = 0; i < 10000000; i++) {
        printf("TASK_A Triggered %d times\n", i);
        __asm__ __volatile__("wfi");
    }
}

void task_C(void)
{
    for (int i = 0; i < 10000000; i++) {
        printf("TASK_C Triggered %d times\n", i);
        __asm__ __volatile__("wfi");
    }
}

void main(void)
{
    heap_init((void *) &__heap_top, (size_t) &__heap_size);
    uart_init(115200);

    printf("\n\n\n\nHI,QEMU\n\n\n");
    init_task_q();
    printf("TASK CREATING....\n");

    create_task((uint32_t) task_A, 1);
    create_task((uint32_t) task_B, 1);
    create_task((uint32_t) task_C, 1);

    printf("TASK CREATING Complete ...\n");

    hal_scheduler_init(kcb->cur_tcb->context);

    while (1) {
        __asm__ __volatile__("wfi");
    };
}
