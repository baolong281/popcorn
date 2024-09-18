#ifndef _KERNEL_UTILS_H
#define _KERNEL_UTILS_H 1

#include <stddef.h>
#include <stdint.h>

// https://wiki.osdev.org/Inline_Assembly/Examples#I/O_access
static inline void outb(uint16_t port, uint8_t val) {
  __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
  /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers
   * that fit in 8b. (N constraint). Wider immediate constants would be
   * truncated at assemble-time (e.g. "i" constraint). The  outb  %al, %dx
   * encoding is the only option for all other cases. %1 expands to %dx because
   * port  is a uint16_t.  %w1 could be used if we had the port number a wider C
   * type */
}

static inline char inb(uint16_t port) {
  char rv;
  __asm__ volatile("inb %w1, %b0" : "=a"(rv) : "Nd"(port));
  return rv;
}

struct InterruptRegisters {
  uint32_t cr2;
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_no, err_code;
  uint32_t eip, cs, eflags, useresp, ss;
};

struct muiltiboot_memory_map {
  unsigned int size;
  unsigned int base_addr_low;
  unsigned int base_addr_high;
  unsigned int length_low;
  unsigned int length_high;
  unsigned int type;
} __attribute__((packed));

#endif
