#include <thread.h>
#include <thr_internals.h>
#include <mutex.h>
#include <cond.h>
#include <linked_list.h>
#include <malloc.h>

#include <syscall.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

// @Author: Armin Zou
// This is the implementation of user thread library, where thr_init function initialize a mutex and
// initialize a thread list for all threads that will be created later to excecute the tasks, notice
// a simple syscall get_esp is being used to get the stack pointer for parent thread. When creating the
// thread, the parent thread first being created with allocated stack space using the aligned stack pointer.
// And the child thread is created with the same size of stack by thread_fork syscall. All of the threads
// are being store into the thread ready list, indicating they are ready for task. The thr_join function
// cleans up after a thread returning the status information provided by the thread at the time of exit. First
// find the corresponding threads that are mean to be join, setting the exit status properly and
//the thread status as dead thread once the target thread is joined. thr_exit function simply find the current
// running thread in the ready list which allows the thread to exit and signal the waiting thread relating
// to the joined thread.

// Uncomment the following line to disable printf calls
#define _REMOVE_PRINTF_CALLS_
#ifdef _REMOVE_PRINTF_CALLS_
#define printf(...)
#endif // _REMOVE_PRINTF_CALLS_

extern void *exception_stack;

/* Helper Function */

// function allocate memory for new ready threads
thread_t* alloc_thread(size_t size, void *head_ptr, void *tail_ptr){
	thread_t *thr = (thread_t*)malloc(sizeof(thread_t));

    // Initialize thread values
    thr->status = THREAD_READY;
    thr->stack_info.stack_size = size;
	thr->stack_info.stack_head = head_ptr;
	thr->stack_info.stack_tail = tail_ptr;
    cond_init(&(thr->join_signal));
    mutex_init(&(thr->thr_mutex));
    return thr;
}

// get tid of thread
void *thread_tid(void *thr){
    return (void*)((thread_t *)thr)->tid;
}

//
int thr_init(unsigned int stack_size)
{
	// check for valid stack size
	assert(!(stack_size <= 0));

	// initialize locks
	mutex_init(&m_mutex);

	// initialize a thread list for all created threads
	list_init(&thread_ready_list);

    // Page align stack size
	thr_stack_size = ((stack_size+(PAGE_SIZE-1)) & ~(PAGE_SIZE-1));

	// get esp and page align it
	void* esp_ptr = get_initial_esp();
	parent_stack_high = (void*)((uint32_t) esp_ptr - ((uint32_t) esp_ptr % PAGE_SIZE));

    return 0;
}

int thr_create(void* (*func)(void*), void* arg)
{
	// initialize variables
	thread_t *ready_threads;

	// check for func and arg
	assert(func != NULL || arg != NULL);

	// set user stack top pointer
	user_top = (void*) (parent_stack_high - thread_ready_list.count * thr_stack_size);

	// allocating memory for new ready thread
    while(new_pages(user_top, thr_stack_size) < 0){
    	user_top = user_top - thr_stack_size;
    }

    // allocate and update thread information
    ready_threads = alloc_thread(thr_stack_size, user_top, user_bot);
    mutex_lock(&(ready_threads->thr_mutex));

    // creating the new ready thread
    int tid = thread_fork(user_top + thr_stack_size, func, arg);
    if (tid < 0){
    	mutex_unlock(&(ready_threads->thr_mutex));
        return -1;
    }
    ready_threads->tid = tid;

    // insert new ready thread into ready list
	if(list_insert(&thread_ready_list, (void*)ready_threads) < 0){
		free(ready_threads);
		return -1;
	}
	mutex_unlock(&(ready_threads->thr_mutex));
	return tid;
}

int thr_join(int tid, void** status)
{
	int join_flag;

	// check condition
	assert(gettid() != tid);

	thread_t *join_thr;
	if(list_find(&thread_ready_list, thread_tid, (void *)tid, (void **)&(join_thr)) < 0){
		printf("No corresponding thread in the ready list\n");
		return -1;
	}
	mutex_lock(&join_thr -> thr_mutex);

	// only join when join_status is WAIT_FOR_JOIN
	if (join_thr -> join_status == WAIT_FOR_JOIN){
		printf("Thread is okay to join\n")
	}
	else{
		// The corresponding thread is already in joined status
		mutex_unlock(&join_thr -> thr_mutex);
		return -1;
	}
	while(1){
		// check if the thread is dead thread
		if(join_thr -> status == THREAD_DEAD){
			mutex_unlock(&join_thr -> thr_mutex);
			join_flag = -1;
			break;
		}
		// the thread is joinable
		else{
			join_thr->join_status = JOINED;
			// wait on the join thread
			cond_wait(&join_thr -> join_signal, &join_thr -> thr_mutex);
			if(status != NULL){
				*status = join_thr -> exit_status;
			}
			// after set up the exit status, mark it as dead
			join_thr->status = THREAD_DEAD;
			join_thr->tid = -1;
			// destory the join signal
		    cond_destroy(&(join_thr->join_signal));
			mutex_unlock(&join_thr -> thr_mutex);
			join_flag = 0;
			break;
		}
	}
	return join_flag;
}

void thr_exit(void* status)
{
	// get current running thread id
	int tid = thr_getid();

	thread_t *thread_to_exit;

	// find the current running thread in the ready list
	while (1){
		// if found, break;
		if (list_find(&thread_ready_list, thread_tid, (void *)tid, (void **)&(thread_to_exit)) == 0){
			break;
		}
		else{
			yield(-1);
			vanish();
		}
	}
	mutex_lock(&thread_to_exit -> thr_mutex);

	// set status of the thread
	thread_to_exit -> exit_status = status;
	thread_to_exit -> status = THREAD_DEAD;

	// signals the corresponding thread
	cond_signal(&thread_to_exit -> join_signal);
	mutex_unlock(&thread_to_exit -> thr_mutex);
	set_status((int) status);
	vanish();

}

int thr_getid(void)
{
    // function return the thread ID of the currently running thread
    return gettid();
}

int thr_yield(int tid)
{
    // function defers execution of the invoking thread to a later time infavor of the thread with ID tid
	return yield(tid);
}

