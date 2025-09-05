#if defined(UNIT_TEST)
#include <stddef.h>
#include <stdint.h>
#else
#include "type.h"
#endif


/* First-fit malloc */
void *malloc(size_t size);
/* Forward coalescing and backward coalescing */
void free(void *ptr);
/* Check available memblock and show address*/
void check_memblock(void);


/* Heap initialization, use memblock to contorl*/
void heap_init(uintptr_t *heap_top, size_t size);
