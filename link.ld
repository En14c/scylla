ENTRY(_kstart)

SECTIONS {
    . = 0x00100000;

    .multi_boot_header :
    {
        *(.multi_boot_header)
    }

    PROVIDE(k_start = .);
    PROVIDE(k_stext = .);
    .text ALIGN(0x1000) :
    {
        *(.text)
    }
    PROVIDE(k_etext = .);

    .rodata ALIGN(0x1000):
    {
        *(.rodata)
    }

    PROVIDE(k_sdata = .);
    .data ALIGN(0x1000):
    {
        *(.data)
    }
    PROVIDE(k_edata = .);

    .bss ALIGN(0x1000):
    {
        *(.bss)
    }

    PROVIDE(k_end = .);
}
