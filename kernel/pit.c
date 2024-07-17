#include <kernel/idt.h>
#include <kernel/pit.h>
#include <kernel/utils.h>
#include <stdint.h>
#include <stdio.h>

uint64_t ticks;

const uint32_t freq = 1;

void handle_irq0(struct InterruptRegisters *regs) {
  ticks++;
  printf("Tick: %d\n", ticks);
}

void init_pit() {
  ticks = 0;
  irq_install_handler(0, &handle_irq0);

  uint32_t divisor = 1193180 / freq;

  // set to square wave generator
  // oscillates between zero and one, setting off interrupt trigger
  outb(PIT_COMMAND, 0x36);
  outb(PIT_DATA0, (uint8_t)(divisor & 0xFF));
  outb(PIT_DATA0, (uint8_t)((divisor >> 8) & 0xFF));
}
