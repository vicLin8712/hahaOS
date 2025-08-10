#include "include/lib/malloc.h"
#include "include/sections.h"
#include "type.h"

#define PAGE_SIZE  4096 /* 4KB per page */

void *page_allocate(size_t page)
{
    char *begin = (char *) __free_ram;
    char *end = begin + page * PAGE_SIZE;
    memset(begin, '0', page * PAGE_SIZE);
}