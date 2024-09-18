.equ MBOOT_PAGE_ALIGN, 1 << 0
.equ MBOOT_MEM_INFO, 1 << 1
.equ MBOOT_USE_GFX, 0
.equ MBOOT_MAGIC, 0x1BADB002
.equ MBOOT_FLAGS, MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_USE_GFX
.equ MBOOT_CHECKSUM, -(MBOOT_MAGIC + MBOOT_FLAGS)

.section .multiboot.data, "aw"
.align 4
    .long MBOOT_MAGIC
    .long MBOOT_FLAGS
    .long MBOOT_CHECKSUM
    .long 0, 0, 0, 0, 0
    .long 0
    .long 800
    .long 600
    .long 32

.section .bss, "aw", @nobits
.align 16
stack_bottom:
    .skip 16384
stack_top:

.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
    movl $(initial_page_dir - 0xC0000000), %ecx
    movl %ecx, %cr3
    movl %cr4, %ecx
    orl $0x10, %ecx
    movl %ecx, %cr4
    movl %cr0, %ecx
    orl $0x80000000, %ecx
    movl %ecx, %cr0
    jmp higher_half

.section .text
higher_half:
    movl $stack_top, %esp
    pushl %ebx
    pushl %eax
    xorl %ebp, %ebp
    .extern kernel_main
    call kernel_main

halt:
    hlt
    jmp halt

.section .data
.align 4096
.global initial_page_dir
initial_page_dir:
    .long 0b10000011
    .fill 768-1, 4, 0
    .long (0 << 22) | 0b10000011
    .long (1 << 22) | 0b10000011
    .long (2 << 22) | 0b10000011
    .long (3 << 22) | 0b10000011
    .fill 256-4, 4, 0
