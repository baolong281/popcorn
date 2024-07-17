.global idt_flush
idt_flush:
    movl 4(%esp), %eax
    lidtl (%eax)
    sti
    ret

.macro ISR_NOERRCODE num
    .global isr\num
    isr\num:
        cli
        pushl $0
        pushl $\num
        jmp isr_common_stub
.endm

.macro ISR_ERRCODE num
    .global isr\num
    isr\num:
        cli
        pushl $\num
        jmp isr_common_stub
.endm

.macro IRQ num, vector
    .global irq\num
    irq\num:
        cli
        pushl $0
        pushl $\vector
        jmp irq_common_stub
.endm

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9 
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 128
ISR_NOERRCODE 177

IRQ 0, 32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

.extern isr_handler
isr_common_stub:
/*
    This interrupt service routine (ISR) common stub handles the low-level mechanics
    of interrupt handling in an x86 system. Here's a high-level overview:

    1. **Save State**: 
       - Save the current state of all general-purpose registers and the segment register (DS).
       - Save the value of the CR2 register, which contains the address causing a page fault.

    2. **Setup Kernel Data Segment**: 
       - Set up the segment registers (DS, ES, FS, GS) to the kernel data segment to ensure
         the handler can access the correct memory locations with kernel privileges.

    3. **Call ISR Handler**: 
       - Pass the current stack pointer to the ISR handler function (`isr_handler`), which will 
         handle the specifics of the interrupt.

    4. **Restore State**: 
       - After the ISR handler completes, restore the original segment registers and 
         the state of all general-purpose registers.

    5. **Resume Execution**: 
       - Re-enable interrupts and return from the interrupt, restoring the CPU state to 
         continue execution of the interrupted program.

    This process ensures that the system can handle interrupts effectively while preserving 
    the state of the CPU and allowing smooth continuation of the interrupted tasks.
*/
    pusha
    movl %ds, %eax
    pushl %eax
    movl %cr2, %eax
    pushl %eax

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    pushl %esp
    call isr_handler

    addl $8, %esp
    popl %ebx
    movw %bx, %ds
    movw %bx, %es
    movw %bx, %fs
    movw %bx, %gs

    popa
    addl $8, %esp
    sti
    iret



.extern irq_handler
irq_common_stub:
    pusha
    movl %ds, %eax
    pushl %eax
    movl %cr2, %eax
    pushl %eax
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    
    pushl %esp
    call irq_handler
    addl $8, %esp
    popl %ebx
    movw %bx, %ds
    movw %bx, %es
    movw %bx, %fs
    movw %bx, %gs
    popa
    addl $8, %esp
    sti
    iret
