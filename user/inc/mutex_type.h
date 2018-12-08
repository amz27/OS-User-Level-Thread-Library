/** @file mutex_type.h
 *  @brief This file defines the type for mutexes.
 *  @Author: Armin Zou
 */

#ifndef _MUTEX_TYPE_H
#define _MUTEX_TYPE_H

typedef struct mutex_s {
	int locked;
} mutex_t;

#endif /* _MUTEX_TYPE_H */
