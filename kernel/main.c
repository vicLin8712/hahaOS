#include "hal.h"
#include "include/sys/task.h"
#include "lib/malloc.h"
#include "libc.h"



void main(void)
{
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
    heap_init((void *)&__heap_top, (size_t)&__heap_size);
    uart_init(115200);

    printf("\n\n\n\nHI,QEMU");
    

    while (1) {
        __asm__ __volatile__("wfi");
    };
}

