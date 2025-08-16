#include "include/lib/malloc.h"
#include "hal.h"
#include "libc.h"
#include "sys/error.h"
#include "sys/task.h"
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
    end = (memblock_t *)((size_t)start + len - sizeof(memblock_t));
    printf("start is %x\n", start);
    printf("end is %x \n", end);

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
    memblock_t *p = first_free;
    size = ALIGN4(size);
    while (p) {
       

        if (!IS_USED(p) && GET_SIZE(p) >= size) {
            size_t remaining = GET_SIZE(p) - size;

            if (remaining >= sizeof(memblock_t) + MIN_MALLOC_SIZE) {
                /* Split block if the space is enough */
                memblock_t *new_block =
                    (memblock_t *) ((size_t) p + sizeof(memblock_t) + size);
                new_block->next = p->next;
                new_block->size = remaining - sizeof(memblock_t);
                MARK_FREE(new_block);
                p->next = new_block;
                p->size = size;
            }

            MARK_USED(p);
            return (void *) (p + 1);
        }
        p = p->next;
    }
    panic(ERR_MALLOC);
}


void free(void *ptr)
{
    if (!ptr)
        return;

    memblock_t *p = (memblock_t *) ptr - 1;
    MARK_FREE(p);

    /* Forward merge */
    if (p->next && !IS_USED(p->next) &&
        (uint8_t *) p + sizeof(memblock_t) + GET_SIZE(p) ==
            (uint8_t *) p->next) {
        p->size = GET_SIZE(p) + sizeof(memblock_t) + GET_SIZE(p->next);
        p->next = p->next->next;
    }

    /* Backward merge */
    memblock_t *prev = NULL;
    memblock_t *current = first_free;
    while (current && current != p) {
        prev = current;
        current = current->next;
    }

    if (prev && !IS_USED(prev) &&
        (uint8_t *) prev + sizeof(memblock_t) + GET_SIZE(prev) ==
            (uint8_t *) p) {
        prev->size = GET_SIZE(prev) + sizeof(memblock_t) + GET_SIZE(p);
        prev->next = p->next;
    }
}

void check_memblock(void)
{
    memblock_t *cur = first_free;
    int i = 0;
    while (cur) {
        printf("Current memblock index is %d, address is %x\n", i + 1, cur);
        cur = cur->next;
        i++;
    }
}