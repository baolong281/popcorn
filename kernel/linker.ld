/* Link.ld -- Linker script for the kernel - ensure everything goes in the */
/*            Correct place.  */
/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */
ENTRY(_start)
SECTIONS
{

    . = 0x00100000;

    _kernel_start = .;

    .multiboot.data : {
      *(.multiboot.data)
    }

    .multiboot.text : {
      *(.multiboot.text)
    }

    . += 0xC0000000;
    .text ALIGN(4K): AT(ADDR(.text) - 0xC0000000){
        *(.text)
    }
    .rodata ALIGN(4K): AT(ADDR(.rodata) - 0xC0000000){
        *(.rodata)
    }
    .data ALIGN(4K): AT(ADDR(.data) - 0xC0000000){
        *(.data)
    }
    .bss ALIGN(4K): AT(ADDR(.bss) - 0xC0000000){
        *(COMMON)
        *(.bss)
    }

    _kernel_end = .;
}
