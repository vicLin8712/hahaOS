#include "type.h"


/* First-fit malloc */
void *malloc(size_t size);

/* Heap initialization, use memblock to contorl*/
void heap_init(uintptr_t *heap_top, size_t size);
