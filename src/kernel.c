#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void empty_last_line() {
  size_t lrow = (VGA_HEIGHT - 1) * VGA_WIDTH;
  for(size_t x=0; x < VGA_WIDTH; x++) {
    terminal_buffer[lrow + x] = vga_entry(' ', terminal_color);
  }
}

void terminal_clear_newline() {
  for(size_t y=0; y < VGA_HEIGHT - 1; y++) {
    size_t org_row = VGA_WIDTH * y;
    size_t next_row = VGA_WIDTH * (y + 1);
    for(size_t x = 0; x < VGA_WIDTH; x++) {
      terminal_buffer[org_row + x] = terminal_buffer[next_row + x];
    }
  }
  empty_last_line();
}

void terminal_putchar(char c) 
{

  if(c == '\n') {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)  {
      terminal_row  = VGA_HEIGHT - 1;
      terminal_clear_newline();
    }
    return;
  }

  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {
    terminal_column  = VGA_WIDTH - 1;
    if (++terminal_row == VGA_HEIGHT)  {
      terminal_row  = VGA_HEIGHT - 1;
      terminal_clear_newline();
    }
  }
}


void terminal_write(const char* data) 
{
  while(*data) {
    terminal_putchar(*data++);
  }
}

void terminal_writestring(const char* data) 
{
	terminal_write(data);
}

void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();

    terminal_writestring("first message\n");
    for(size_t i = 0; i < VGA_HEIGHT; i++) {
      terminal_writestring("Hello, kernel World!\n");
    }
    terminal_writestring("free big don\n");
    terminal_writestring("test string\n");
    terminal_writestring("wilco\n");
    terminal_writestring("yankee hotel foxtrot\n");
    terminal_writestring("shoutout bladee\n");
}
