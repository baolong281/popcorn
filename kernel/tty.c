#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/vga.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *)0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

void terminal_initialize(void) {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
  terminal_buffer = VGA_MEMORY;
  uint16_t empty_val = vga_entry_color(' ', terminal_color);
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    size_t ind = y * VGA_WIDTH;
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      terminal_buffer[ind + x] = empty_val;
    }
  }
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_clear_line() {
  for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
    uint16_t *ind = terminal_buffer + y * VGA_WIDTH;
    uint16_t *next_ind = terminal_buffer + (y + 1) * VGA_WIDTH;
    memcpy((void *)ind, (const void *)next_ind, VGA_WIDTH);
  }

  size_t ind = (VGA_HEIGHT - 1) * VGA_WIDTH;
  uint16_t empty = vga_entry(' ', terminal_color);
  for (size_t i = 0; i < VGA_WIDTH; i++)
    terminal_buffer[ind + i] = empty;
}

void terminal_putchar(char c) {
  unsigned char uc = c;
  if (c == '\n') {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
      terminal_row = VGA_HEIGHT - 1;
      terminal_clear_line();
    }
    return;
  }
  terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
      terminal_row = VGA_HEIGHT - 1;
  }
}

void terminal_write(const char *data) {
  while (*data) {
    terminal_putchar(*data++);
  }
}

void terminal_writestring(const char *data) { terminal_write(data); }
