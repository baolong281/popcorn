#include "include/gdt.h"
#include <kernel/gdt.h>
#include <string.h>

#define GDT_ENTRIES 6

struct gdt_entry gdt_entries[GDT_ENTRIES];
struct gdt_ptr gdt_ptr;
struct tss_entry tss_entry;

void gdt_flush(struct gdt_ptr *ptr) {
  __asm__ __volatile__("lgdt (%0)\n\t"
                       "movl $0x10, %%eax\n\t"
                       "movl %%eax, %%ds\n\t"
                       "movl %%eax, %%es\n\t"
                       "movl %%eax, %%fs\n\t"
                       "movl %%eax, %%gs\n\t"
                       "movl %%eax, %%ss\n\t"
                       "ljmp $0x08, $1f\n\t"
                       "1:\n\t"
                       :
                       : "r"(ptr)
                       : "eax", "memory");
}

void tss_flush() { __asm__ __volatile__("ltr %ax\n\t"); }

void init_gdt() {
  gdt_ptr.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
  gdt_ptr.base = (unsigned int)&gdt_entries;

  // https://wiki.osdev.org/Global_Descriptor_Table
  // null segment
  set_gdt_gate(0, 0, 0, 0, 0);
  // kernel code segment
  // 0x9A = 1001 1010
  set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
  // kernel data segment
  // 0x92 = 1001 0010
  set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
  // user code segment
  // 0xFA = 1111 1010
  // set level to ring 3
  set_gdt_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
  // user data segment
  // 0xF2 = 1111 0010
  set_gdt_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
  write_tss(5, 0x10, 0x0);

  tss_flush();
  gdt_flush(&gdt_ptr);
}

void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access,
                  uint8_t granularity) {
  // get lower 16 bits of base
  gdt_entries[num].base_low = (base & 0xFFFF);
  // shift base 16 bits to the right and get the lower 8 bits
  gdt_entries[num].base_mid = (base >> 16) & 0xFF;
  // shift base 24 bits to the right and get the lower 8 bits
  gdt_entries[num].base_high = (base >> 24) & 0xFF;

  gdt_entries[num].limit = (limit & 0xFFFF);
  gdt_entries[num].flags = (limit >> 16) & 0x0F;

  gdt_entries[num].flags |= granularity & 0xF0;
  gdt_entries[num].access = access;
}

void write_tss(uint32_t num, uint16_t ss0, uint32_t esp0) {
  uint32_t base = (uint32_t)&tss_entry;
  uint32_t limit = base + sizeof(struct tss_entry);

  // 0xE9 = 1110 1001
  set_gdt_gate(num, base, limit, 0xE9, 0x00);

  memset(&tss_entry, 0, sizeof(struct tss_entry));

  tss_entry.ss0 = ss0;
  tss_entry.esp0 = esp0;

  tss_entry.cs = 0x08 | 0x03;
  tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs =
      0x10 | 0x03;
}
