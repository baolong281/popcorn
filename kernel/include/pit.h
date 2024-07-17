#ifndef _KERNEL_PIT_H
#define _KERNEL_PIT_H 1

#include <kernel/utils.h>

#define PIT_COMMAND 0x43
#define PIT_DATA0 0x40

void init_pit();
void handle_irq0(struct InterruptRegisters *regs);

#endif
