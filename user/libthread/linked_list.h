#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

// @Author: Armin Zou

typedef struct list_node {
    struct list_node* next;
    //struct list_node* prev;
    void* data;
} list_node_t;

typedef struct {
    list_node_t* head;
    list_node_t* tail;
    size_t count;
} list_t;

size_t list_count(list_t* list);

int list_init(list_t *list);

void list_destroy(list_t* list);

int list_find(list_t* list, void* (*func)(void*), void* data, void** data_ptr);

int list_insert(list_t* list, void* data);

int list_delete(list_t* list, void **data);


#endif
