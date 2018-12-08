/** @file thr_internals.h
 *  
 *  @Author: Armin Zou
 *  @brief This file may be used to define things
 *         internal to the thread library.
 */
#include <stdbool.h>
#include <mutex_type.h>
#include <cond_type.h>
#include <linked_list.h>
#include <stdint.h>
#include <types.h>
#include <assert.h>

#ifndef THR_INTERNALS_H
#define THR_INTERNALS_H

/* ADD OTHER LIBTHREAD-RELATED HEADER CONTENT HERE */

// globle variables
size_t thr_stack_size;
list_t thread_ready_list;

// stack pointers
void* parent_stack_high;		// parent thread stack top pointer
void *user_top;					// user stack space top pointer
void *user_bot;					// user stack space bot pointer

// mutex
mutex_t m_mutex;

// syscalls
void* get_initial_esp(void);
int thread_fork(void *new_esp, void *(*func)(void*), void *args);

typedef struct stack_s{
	unsigned int stack_size;
	void *stack_head;
	void *stack_tail;
} stack_t;

enum thread_status
  {
    THREAD_READY = 0,		// ready to run
    THREAD_DEAD = 1,		// dead thread
	THREAD_RUNNING = 2		// running thread
  };

enum join_status_t
  {
	WAIT_FOR_JOIN = 1,		// can be join
	JOINED = 0				// already joined
  };

typedef struct thread_s{
    enum thread_status status;	// thread status
	enum join_status_t join_status;	// join status
	stack_t stack_info;	// stack information
	int tid;			// thread id
	int reject;			// rejct flag
	void *exit_status;	// exit status
	mutex_t thr_mutex;	// thread mutex
	cond_t join_signal;		// condition variable to signal joining threads
} thread_t;

#endif /* THR_INTERNALS_H */
