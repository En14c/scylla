ENTRY(_kstart)

SECTIONS {
    . = 0xc0100000;
    
    .multi_boot_header ALIGN(0x04) : AT(ADDR(.multi_boot_header) - 0xc0000000)
    {
        *(.multi_boot_header)
    }

    PROVIDE(k_start = .);
    PROVIDE(k_stext = .);

    .text ALIGN(0x1000) : AT(ADDR(.text) - 0xc0000000)
    {
        *(.text)
    }

    PROVIDE(k_etext = .);

    .rodata ALIGN(0x1000): AT(ADDR(.rodata) - 0xc0000000)
    {
        *(.rodata)
    }

    PROVIDE(k_sdata = .);

    .data ALIGN(0x1000): AT(ADDR(.data) - 0xc0000000)
    {
        *(.data)
    }

    PROVIDE(k_edata = .);

    .bss ALIGN(0x1000): AT(ADDR(.bss) - 0xc0000000)
    {
        *(.bss)
    }

    PROVIDE(k_end = .);
}
