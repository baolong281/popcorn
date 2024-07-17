#include "include/utils.h"
#include <kernel/idt.h>
#include <kernel/utils.h>
#include <stdio.h>
#include <string.h>

__attribute__((aligned(0x10))) static struct idt_entry idt[256];

struct idt_descriptor_idtr idtr;

extern void idt_flush(uint32_t);

char *exception_msgs[] = {"Division By Zero",
                          "Debug",
                          "Non Maskable Interrupt",
                          "Breakpoint",
                          "Into Detected Overflow",
                          "Out of Bounds",
                          "Invalid Opcode",
                          "No Coprocessor",
                          "Double fault",
                          "Coprocessor Segment Overrun",
                          "Bad TSS",
                          "Segment not present",
                          "Stack fault",
                          "General protection fault",
                          "Page fault",
                          "Unknown Interrupt",
                          "Coprocessor Fault",
                          "Alignment Fault",
                          "Machine Check",
                          "Reserved",
                          "Reserved",
                          "Reserved",
                          "Reserved",
                          "Reserved",
                          "Reserved",
                          "Reserved",
                          "Reserved",
                          "Reserved",
                          "Reserved",
                          "Reserved",
                          "Reserved",
                          "Reserved"};

void isr_handler(struct InterruptRegisters *regs) {
  if (regs->int_no < 32) {
    printf("\n");
    puts("Exception. System Halted!");
    puts((const char *)exception_msgs[regs->int_no]);
    // print all registers
    printf("eax: %x\n", regs->eax);
    printf("ebx: %x\n", regs->ebx);
    printf("ecx: %x\n", regs->ecx);
    printf("edx: %x\n", regs->edx);
    printf("esi: %x\n", regs->esi);
    printf("edi: %x\n", regs->edi);
    printf("ebp: %x\n", regs->ebp);
    printf("esp: %x\n", regs->esp);
    printf("eip: %x\n", regs->eip);
    printf("cs: %x\n", regs->cs);
    printf("ds: %x\n", regs->ds);
    printf("ss: %x\n", regs->ss);
    printf("eflags: %x\n", regs->eflags);
    printf("int_no: %x\n", regs->int_no);
    printf("err_code: %x\n", regs->err_code);
    printf("cr2: %x\n", regs->cr2);
    for (;;)
      ;
  }
}

void init_idt() {
  idtr.size = sizeof(struct idt_entry) * 256 - 1;
  idtr.offset = (uint32_t)&idt;

  memset(&idt, 0, sizeof(struct idt_entry) * 256);

  // PIC interrupts
  // https://wiki.osdev.org/8259_PIC
  outb(PIC1_COMMAND, 0x11);
  outb(PIC2_COMMAND, 0x11);

  outb(PIC1_DATA, 0x20);
  outb(PIC2_DATA, 0x28);

  outb(PIC1_DATA, 0x04);
  outb(PIC2_DATA, 0x02);

  outb(PIC1_DATA, 0x01);
  outb(PIC2_DATA, 0x01);

  outb(PIC1_DATA, 0x00);
  outb(PIC2_DATA, 0x00);

  set_idt_gate(0, (uint32_t)isr0, 0x08, 0x8E);
  set_idt_gate(1, (uint32_t)isr1, 0x08, 0x8E);
  set_idt_gate(2, (uint32_t)isr2, 0x08, 0x8E);
  set_idt_gate(3, (uint32_t)isr3, 0x08, 0x8E);
  set_idt_gate(4, (uint32_t)isr4, 0x08, 0x8E);
  set_idt_gate(5, (uint32_t)isr5, 0x08, 0x8E);
  set_idt_gate(6, (uint32_t)isr6, 0x08, 0x8E);
  set_idt_gate(7, (uint32_t)isr7, 0x08, 0x8E);
  set_idt_gate(8, (uint32_t)isr8, 0x08, 0x8E);
  set_idt_gate(9, (uint32_t)isr9, 0x08, 0x8E);
  set_idt_gate(10, (uint32_t)isr10, 0x08, 0x8E);
  set_idt_gate(11, (uint32_t)isr11, 0x08, 0x8E);
  set_idt_gate(12, (uint32_t)isr12, 0x08, 0x8E);
  set_idt_gate(13, (uint32_t)isr13, 0x08, 0x8E);
  set_idt_gate(14, (uint32_t)isr14, 0x08, 0x8E);
  set_idt_gate(15, (uint32_t)isr15, 0x08, 0x8E);
  set_idt_gate(16, (uint32_t)isr16, 0x08, 0x8E);
  set_idt_gate(17, (uint32_t)isr17, 0x08, 0x8E);
  set_idt_gate(18, (uint32_t)isr18, 0x08, 0x8E);
  set_idt_gate(19, (uint32_t)isr19, 0x08, 0x8E);
  set_idt_gate(20, (uint32_t)isr20, 0x08, 0x8E);
  set_idt_gate(21, (uint32_t)isr21, 0x08, 0x8E);
  set_idt_gate(22, (uint32_t)isr22, 0x08, 0x8E);
  set_idt_gate(23, (uint32_t)isr23, 0x08, 0x8E);
  set_idt_gate(24, (uint32_t)isr24, 0x08, 0x8E);
  set_idt_gate(25, (uint32_t)isr25, 0x08, 0x8E);
  set_idt_gate(26, (uint32_t)isr26, 0x08, 0x8E);
  set_idt_gate(27, (uint32_t)isr27, 0x08, 0x8E);
  set_idt_gate(28, (uint32_t)isr28, 0x08, 0x8E);
  set_idt_gate(29, (uint32_t)isr29, 0x08, 0x8E);
  set_idt_gate(30, (uint32_t)isr30, 0x08, 0x8E);
  set_idt_gate(31, (uint32_t)isr31, 0x08, 0x8E);

  set_idt_gate(32, (uint32_t)irq0, 0x08, 0x8E);
  set_idt_gate(33, (uint32_t)irq1, 0x08, 0x8E);
  set_idt_gate(34, (uint32_t)irq2, 0x08, 0x8E);
  set_idt_gate(35, (uint32_t)irq3, 0x08, 0x8E);
  set_idt_gate(36, (uint32_t)irq4, 0x08, 0x8E);
  set_idt_gate(37, (uint32_t)irq5, 0x08, 0x8E);
  set_idt_gate(38, (uint32_t)irq6, 0x08, 0x8E);
  set_idt_gate(39, (uint32_t)irq7, 0x08, 0x8E);
  set_idt_gate(40, (uint32_t)irq8, 0x08, 0x8E);
  set_idt_gate(41, (uint32_t)irq9, 0x08, 0x8E);
  set_idt_gate(42, (uint32_t)irq10, 0x08, 0x8E);
  set_idt_gate(43, (uint32_t)irq11, 0x08, 0x8E);
  set_idt_gate(44, (uint32_t)irq12, 0x08, 0x8E);
  set_idt_gate(45, (uint32_t)irq13, 0x08, 0x8E);
  set_idt_gate(46, (uint32_t)irq14, 0x08, 0x8E);
  set_idt_gate(47, (uint32_t)irq15, 0x08, 0x8E);

  set_idt_gate(128, (uint32_t)isr128, 0x08, 0x8E);
  set_idt_gate(177, (uint32_t)isr177, 0x08, 0x8E);

  idt_flush((uint32_t)&idtr);
}

void set_idt_gate(uint8_t num, uint32_t base, uint16_t selector,
                  uint8_t type_attr) {
  idt[num].offset_low = base & 0xFFFF;
  idt[num].offset_high = (base >> 16) & 0xFFFF;
  idt[num].selector = selector;
  idt[num].zero = 0;
  idt[num].type_attr = type_attr | 0x60;
}

void *irq_routines[16] = {0};

void irq_install_handler(int irq,
                         void (*handler)(struct InterruptRegisters *)) {
  irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) { irq_routines[irq] = 0; }

void irq_handler(struct InterruptRegisters *regs) {
  void (*handler)(struct InterruptRegisters *);

  // irq starts at 32, so subtract 32 to index into the array
  handler = irq_routines[regs->int_no - 32];
  if (handler) {
    handler(regs);
  }

  // send end of interrupt signal to the PIC
  if (regs->int_no >= 40) {
    outb(PIC2_COMMAND, 0x20);
  }

  // send end of interrupt signal to the PIC
  outb(PIC1_COMMAND, 0x20);
}
