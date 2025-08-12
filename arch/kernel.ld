ENTRY(boot)

_heap_size = _heap_bottom-_heap_start;

SECTIONS {
    . = 0x80200000;

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

    _heap_start = .;
    . += 64 * 1024 * 1024; /* 64 MB memory */
    _heap_bottom = .;

    . = ALIGN(4);
    . += 128 * 1024; /* 128KB */
    __stack_top = .;


}
