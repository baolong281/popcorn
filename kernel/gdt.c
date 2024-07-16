#include <kernel/gdt.h>

struct gdt_entry gdt_entries[5];
struct gdt_ptr gdt_ptr;

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

void init_gdt() {
  gdt_ptr.limit = (sizeof(struct gdt_entry) * 5) - 1;
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
