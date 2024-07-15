#ifndef _STRING_H
#define _STRING_H 1

#include <stddef.h>

#ifdef __cplusplus
#endif

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict dest, const void* __restrict src, size_t n);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);

#ifdef __cplusplus
#endif

#endif

