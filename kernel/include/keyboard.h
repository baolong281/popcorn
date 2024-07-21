#ifndef _KERNEL_KEYBOARD_H
#define _KERNEL_KEYBOARD_H 1

void init_keyboard();
void keyboard_handler(struct InterruptRegisters *r);

#endif
