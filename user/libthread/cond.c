#include <cond.h>
#include <syscall.h>
#include <linked_list.h>
#include <thr_internals.h>
#include <stddef.h>

// @Author: Armin Zou
// This is implementation of conditional variables, which simply built base on the implementation of
// mutex. The condition variable allows a thread to voluntarily relinquish the CPU so that other threads
// may make changes to the shared state.

// Uncomment the following line to disable printf calls
//#define _REMOVE_PRINTF_CALLS_
#ifdef _REMOVE_PRINTF_CALLS_
#define printf(...)
#endif // _REMOVE_PRINTF_CALLS_

int cond_init(cond_t* cond)
{
	// check if cond is valid
	if(cond == NULL){
		return -1;
	}

	// initialize the mutex for cond
	if(mutex_init(&cond->cv) < 0){
		return -1;
	}

	// initialize the queue
	if(list_init(&cond->queue) < 0){
		return -1;
	}
    return 0;
}

void cond_destroy(cond_t* cond)
{
	assert(cond != NULL);

	// lock the mutex before destroy the queue then unlock and destroy the mutex
	mutex_lock(&(cond->cv));
	list_destroy(&(cond->queue));
	mutex_unlock(&(cond->cv));
	mutex_destroy(&(cond->cv));
}

void cond_wait(cond_t* cond, mutex_t* mutex)
{
	// check conditions
	assert(cond != NULL || mutex != NULL);
	assert(mutex != NULL);

	mutex_lock(&(cond->cv));
	thread_t thread;
	thread.tid = gettid();
	thread.reject = 0;

	list_insert(&(cond->queue), &thread);
	mutex_unlock(mutex);

	mutex_unlock(&(cond->cv));

	while(!(thread.reject)){
		deschedule(&(thread.reject));
	}
	mutex_lock(mutex);
}

void cond_signal(cond_t* cond)
{
	assert(cond != NULL);

	// lock the cv
	mutex_lock(&(cond->cv));
	thread_t* thread;

	// delete the thread from the queue
	// if delete failed, unlock and return nothing
	if(list_delete(&(cond->queue), (void**)(&thread)) < 0){
		mutex_unlock(&(cond->cv));
		return;
	}

	// wake up the thread
	thread->reject = 1;
	make_runnable(thread->tid);
	mutex_unlock(&(cond->cv));
}

void cond_broadcast(cond_t* cond)
{
	assert(cond != NULL);

	// lock the cv
	mutex_lock(&(cond->cv));
	thread_t *thr;

	// wake up all thread by deleting all thread from the queue
	while(list_delete(&(cond->queue), (void**)(&thr)) == 0){
		thr->reject = 1;
		make_runnable(thr->tid);
	}
	mutex_unlock(&(cond->cv));
}
