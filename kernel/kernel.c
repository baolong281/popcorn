#include <kernel/idt.h>
#include <kernel/keyboard.h>
#include <kernel/multiboot.h>
#include <kernel/pit.h>
#include <kernel/utils.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/gdt.h>
#include <kernel/tty.h>
#include <stdio.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error                                                                         \
    "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */

void kernel_main(uint32_t magic, struct multiboot_info *mbd) {
  terminal_initialize();
  init_gdt();
  init_idt();
  init_pit();
  init_keyboard();
  /* Make sure the magic number matches for memory mapping*/
  printf("mods_addr: %x\n", mbd);
  printf("mods_addr: %x\n", mbd->mods_addr);
  printf("keyboard initialized\n");
  for (;;) {
  }
}
