ENTRY(boot)


SECTIONS {
    . = 0x80000000;

    .text :{
        KEEP(*(.text.boot));
        *(.text .text.*);
    }

    .rodata : ALIGN(4) {
        *(.rodata .rodata.*);
    }

    .data : ALIGN(4) {
        *(.data .data.*);
    }

    .bss : ALIGN(4) {
        __bss = .;
        *(.bss .bss.* .sbss .sbss.*);
        __bss_end = .;
    }

    . = ALIGN(4);
    . += 64 * 1024 *1024; /* 128MB */
    __stack_top = .;
    . -= 4 *1024 *1024;
    __stack_end =.;


    /* Heap */
    __heap_end =.;
    __heap_top = __bss_end;
    __heap_size = __heap_end-__heap_top;

}
