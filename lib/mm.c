#include "include/libc.h"
#include "include/sections.h"

#define PAGE_SIZE  4096 /* 4KB per page */

void *memset(void *buf, char c, size_t n)
{
    uint8_t *p = (uint8_t *) buf;
    while (n--){
        *p++ = c;
        printf("memory address %x is reset\n", p);
    }
    return buf;
}

void *memcpy(char *dst, const char *src, size_t n)
{
    uint8_t *d = (uint8_t *) dst;
    const uint8_t *s = (const uint8_t *) src;
    while (n--) {
        *d++ = *s++;
    }
    return dst;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        if (*s1 != *s2)
            break;
        s1++;
        s2++;
    }
    return *(unsigned char *) s1 - *(unsigned char *) s2;
}

void *strcpy(char *dst, const char *src )
{
    while (*src)
    {
        *dst = *src;
        dst++;
        src++;
    }
    return dst;
}

void *page_allocate(size_t page)
{
    char *begin = (char *) __free_ram;
    char *end = begin + page * PAGE_SIZE;
    memset(begin, '0', page * PAGE_SIZE);
}