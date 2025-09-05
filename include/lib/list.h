#pragma once
#include "lib/malloc.h"

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif

/* List node definition */
typedef struct list_node {
    struct list_node *next;
    void *data;
} list_node_t;

/* List structure, use @head and @tail to maintain whole list */
typedef struct {
    list_node_t *head;
    list_node_t *tail;
    size_t length;
} list_t;

static inline int list_is_empty(const list_t *list)
{
    return !list || list->length == 0U;
}

static inline list_t *list_init(void)
{
    list_t *list = malloc(sizeof(list_t));

    list_node_t *head = malloc(sizeof(list_node_t));
    list_node_t *tail = malloc(sizeof(list_node_t));

    head->next = tail;
    head->data = NULL;

    tail->next = NULL;
    tail->data = NULL;

    list->head = head;
    list->tail = tail;
    list->length = 0U;

    return list;
}

/* Pop and push data */
static inline void *list_pop(list_t *list)
{
    if (unlikely(!list || list->head->next == list->tail))
        return NULL;

    list_node_t *first = list->head->next;
    list->head->next = first->next;
    void *data = first->data;
    free(first);

    list->length--;
    return data;
}

static inline void *list_push(list_t *list, void *data)
{
    if (unlikely(!list || !data))
        return NULL;

    list_node_t **new_node = &list->head;
    while ((*new_node)->next != list->tail)
        new_node = &(*new_node)->next;

    (*new_node)->next = malloc(sizeof(list_node_t));
    (*new_node)->next->data = data;
    (*new_node)->next->next = list->tail;
    list->length++;
}

/* Clear and destroy list */
static inline void list_clear(list_t *list)
{
    if (unlikely(!list))
        return;
    while (!list_is_empty(list))
        list_pop(list);
}

static inline void list_destroy(list_t *list)
{
    if (unlikely(!list))
        return;
    list_clear(list);
    free(list->head);
    free(list->tail);
    free(list);
}
