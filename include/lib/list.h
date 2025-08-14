#pragma once
#include "type.h"
#include "lib/malloc.h"


/* List node definition */
typedef struct list_node
{
    list_node_t *next;
    void *data;
}list_node_t;

/* List structure */
typedef struct 
{
    list_node_t *head;
    list_node_t *tail;
    size_t length;
}list_t;

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



