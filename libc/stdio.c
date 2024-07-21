#include <kernel/tty.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

int putchar(int ic) {
  // throw errors in the future
  char c = (char)ic;
  terminal_putchar(c);
  return ic;
}

int puts(const char *s) {
  terminal_writestring(s);
  terminal_putchar('\n');
  return 1;
}

int printf(const char *__restrict format, ...) {
  va_list args;
  va_start(args, format);

  size_t written = 0;
  size_t ind = 0;

  while (format[ind]) {
    if (format[ind] != '%' || format[ind + 1] == 0) {
      putchar(format[ind]);
      ind++;
      written++;
      continue;
    }

    ind++;
    switch (format[ind]) {
    case 'c': {
      char c = (char)va_arg(args, int);
      putchar(c);
      written++;
      ind++;
      break;
    }
    case 'd': {
      ind++;
      int n = va_arg(args, int);
      if (n == 0) {
        putchar('0');
        written++;
        break;
      }

      bool is_negative = n < 0;
      n = is_negative ? -n : n;

      char buffer[11];
      int i = 0;
      while (n > 0) {
        buffer[i++] = '0' + (n % 10);
        n /= 10;
      }

      if (is_negative) {
        buffer[i++] = '-';
      }

      while (i > 0) {
        putchar(buffer[--i]);
        written++;
      }
      break;
    }
    case 's': {
      const char *s = va_arg(args, const char *);
      while (*s) {
        putchar(*s++);
        written++;
      }
      ind++;
      break;
    }
    case 'x': {
      ind++;
      int n = va_arg(args, int);
      if (n == 0) {
        putchar('0');
        putchar('x');
        putchar('0');
        written += 3;
        break;
      }
      char buffer[12];
      int i = 2;
      while (n > 0) {
        int rem = n % 16;
        if (rem < 10) {
          buffer[i++] = '0' + rem;
        } else {
          buffer[i++] = 'a' + (rem - 10);
        }
        n /= 16;
      }
      buffer[i++] = 'x';
      buffer[i++] = '0';
      while (i > 0) {
        putchar(buffer[--i]);
        written++;
      }

      break;
    }
    default: {
      putchar('%');
      putchar(format[ind]);
      written++;
      written++;
      ind++;
    }
    }
  }
  va_end(args);
  return written;
}
