/** @file cond_type.h
 *  @brief This file defines the type for condition variables.
 *  @Author: Armin Zou
 */

#ifndef _COND_TYPE_H
#define _COND_TYPE_H
#include <linked_list.h>
#include <mutex.h>


typedef struct cond_s {
	mutex_t cv;
    list_t queue;
} cond_t;

#endif /* _COND_TYPE_H */
