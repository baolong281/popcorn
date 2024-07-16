#include <stddef.h>
#include <stdint.h>
#include <string.h>

int memcmp(const void *str1, const void *str2, size_t n) {
  // https://github.com/gcc-mirror/gcc/blob/master/libiberty/memcmp.c
  register const unsigned char *s1 = (const unsigned char *)str1;
  register const unsigned char *s2 = (const unsigned char *)str2;

  while (n-- > 0) {
    if (*s1++ != *s2++) {
      return s1[-1] < s2[-1] ? -1 : 1;
    }
  }
  return 0;
}

void *memcpy(void *__restrict dest, const void *__restrict src, size_t size) {
  // actual implementation has some optimizations
  // do later
  // https://stackoverflow.com/questions/17591624/understanding-the-source-code-of-memcpy
  char *d = dest;
  const char *s = src;
  while (size--) {
    *d++ = *s++;
  }
  return dest;
}

// https://opensource.apple.com/source/network_cmds/network_cmds-481.20.1/unbound/compat/memmove.c.auto.html
void *memmove(void *dest, const void *src, size_t n) {

  uint8_t *from = (uint8_t *)src;
  uint8_t *to = (uint8_t *)dest;

  if (n == 0 || from == to) {
    return dest;
  }

  if (to > from && to - from < n) {
    /* to overlaps with from */
    /*  <from......>         */
    /*         <to........>  */
    /* copy in reverse, to avoid overwriting from */
    uint8_t i;
    for (i = n - 1; i >= 0; i--)
      to[i] = from[i];
    return dest;
  }

  if (from > to && from - to < n) {
    /* to overlaps with from */
    /*        <from......>   */
    /*  <to........>         */
    /* copy forwards, to avoid overwriting from */
    size_t i;
    for (i = 0; i < n; i++)
      to[i] = from[i];
    return dest;
  }

  memcpy(dest, src, n);
  return dest;
}

void *memset(void *dest, int c, size_t n) {
  unsigned char *ptr = (unsigned char *)dest;
  unsigned char value = (unsigned char)c;

  while (n--) {
    *ptr++ = value;
  }

  return dest;
}

size_t strlen(const char *str) {
  size_t i = 0;
  while (*str++)
    i++;
  return i;
}
