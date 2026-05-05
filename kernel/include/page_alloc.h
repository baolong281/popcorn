#include <stdint.h>

extern uint32_t _kernel_end;

static uint32_t kalloc_frame_int();
static void kfree_frame(uint32_t frame);
