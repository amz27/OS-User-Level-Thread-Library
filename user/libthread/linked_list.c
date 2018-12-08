#include <linked_list.h>
#include <stdio.h>
#include "assert.h"

// @Author: Armin Zou
// This is implementation of linked list

// initialize a list
int list_init(list_t *list) {
    if (list == NULL){
    	return -1;
    }
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return 0;
}

// destroy a list
void list_destroy(list_t* list)
{
    if (list == NULL) return;
    list_node_t *ptr = list->head;
    list_node_t *next;
    while (ptr != NULL){
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
}

// Return the number of elements in the list
size_t list_count(list_t* list)
{
	size_t size = 0;
	list_node_t* curr = list->head;
	while(curr){
		size++;
		curr = curr->next;
	}
    return size;
}

// find the corresponding node with given data
int list_find(list_t* list, void* (*func)(void*), void* data, void** data_ptr)
{
	if(list == NULL || func == NULL){
		printf("No list or no function\n");
		return -1;
	}

	list_node_t* current = list->head;

	while(current != NULL){
		if((*func)(current->data) == data){
			*data_ptr = current->data;
			return 0;
		}
		current = current->next;
	}
    return -1;
}

// insert a new node in the list with the given data
int list_insert(list_t* list, void* data)
{
	if(list == NULL){
		printf("List does not exist\n");
		return -1;
	}
    
	list_node_t* new_node = (list_node_t*)malloc(sizeof(list_node_t));
	if(new_node == NULL){
		printf("Error allocating new node\n");
		return -1;
	}

	new_node->next = NULL;
	new_node->data = data;

	if(list->head == NULL){
		list->head = new_node;
		list->tail = new_node;
	}
	else{
		list->tail->next = new_node;
		list->tail = new_node;
	}
	list->count++;
	return 0;
	
}

// delete the node
int list_delete(list_t* list, void **data){
    if (list == NULL){
        return -1;
    }

    list_node_t* head = list->head;
    if (list->count == 0)
        return -1;
    if (head == list->tail){
    	list->tail = NULL;
    }
    list->head = head->next;
    *data = head->data;
    free(head);
    list->count--;
    return 0;
}
