#pragma once
#include "lib/malloc.h"
#include "type.h"


/* List node definition */
typedef struct list_node {
    list_node_t *next;
    void *data;
} list_node_t;

/* List structure, use @head and @tail to maintain whole list */
typedef struct {
    list_node_t *head;
    list_node_t *tail;
    size_t length;
} list_t;

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
/* Insert new node @new at the end of the @list */
static inline void *list_add_tail(list_t *list, list_node_t *new)
{
    if (unlikely(!list || new))
        return NULL;
    list_node_t *node = list->head;
    while (node->next != list->tail)
        node = node->next;

    node->next = new;
    new->next = list->tail;
    list->length++;
}
/* Insert new node @new at the head of the @list */
static inline void *list_add_head(list_t *list, list_node_t *new)
{
    if (unlikely(!list || new))
        return NULL;
    new->next = list->head->next;
    list->head->next = new;
    list->length++;
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
