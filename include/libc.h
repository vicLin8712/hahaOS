#include "hal.h"
#include "type.h"

void *memset(void *buf, char c, size_t n);
void *memcpy(char *dst, const char *src, size_t n);
int strcmp(const char *s1, const char *s2);
void *strcpy(char *dst, const char *src);
void *page_allocate(size_t page);

void putchar(char ch);
void printf(const char *fmt, ...);