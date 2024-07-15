#include <stddef.h>
#include <string.h>

void* memcpy(void* __restrict dest, const void* __restrict src, size_t size) {
  // actual implementation has some optimizations
  // do later https://stackoverflow.com/questions/17591624/understanding-the-source-code-of-memcpy
  char* d = dest;
  const char* s = src;
  while (size--) {
    *d++ = *s++;
  }
  return dest;  
}
  


