#include "include/lib/malloc.h"
#include "hal.h"
#include "libc.h"
#include "type.h"

/* Define memory control block */
typedef struct __memblock {
    struct __memblock *next;
    size_t size;  // Last bit used for control flag as below defined
} memblock_t;

/* Define memory block flags */
#define IS_USED(x) ((x)->size & 1L)      // Check LSB
#define GET_SIZE(x) ((x)->size & ~1L)    // Get size without flag
#define MARK_USED(x) ((x)->size |= 1L)   // Set flag 1
#define MARK_FREE(x) ((x)->size &= ~1L)  // Set flag 0

/* Minimum malloc size*/
#define MIN_MALLOC_SIZE 4

/* Record memblock */
static memblock_t *first_free;
static void *heap_start, *heap_end;

void heap_init(uintptr_t *heap_top, size_t len)
{
    memblock_t *start, *end;


    /* Align heap size to 4 bytes */
    len = ALIGN4(len);

    /* Assign start and end memory control block as initial */
    start = (memblock_t *) heap_top;
    end = (memblock_t *) ((size_t) start + len - sizeof(memblock_t));

    /* Connect each memblock */
    start->next = end;
    end->next = NULL;

    /* Assign each mem size */
    start->size = len - 2 * sizeof(memblock_t);
    end->size = 0;

    /* Setup each memblock status */
    MARK_FREE(start);
    MARK_USED(end);

    first_free = start;
    heap_start = (void *) heap_top;
    heap_end = (void *) ((size_t) end + sizeof(memblock_t));
}

void *malloc(size_t size)
{
    memblock_t *c = first_free;
    while (c) {
        if (!IS_USED(c) && GET_SIZE(c) >= size) {
            size_t remaining = GET_SIZE(c) - size;

            if (remaining >= sizeof(memblock_t) + MIN_MALLOC_SIZE) {
                /* Split block if the space is enough */
                memblock_t *new_block =
                    (memblock_t *) ((size_t) c + sizeof(memblock_t) + size);
                new_block->next = c->next;
                new_block->size = remaining - sizeof(memblock_t);
                MARK_FREE(new_block);

                c->next = new_block;
                c->size = size;
            }

            MARK_USED(c);
            return (void *) (c + 1);
        }
        c = c->next;
    }
}