/** @file sem_type.h
 *  @brief This file defines the type for semaphores.
 *  @Author: Armin Zou
 */

#ifndef _SEM_TYPE_H
#define _SEM_TYPE_H
#include <mutex.h>
#include <cond.h>
#include <linked_list.h>

typedef struct sem_s {
	cond_t sem_m;
	list_t queue;
	int count;
} sem_t;

#endif /* _SEM_TYPE_H */
