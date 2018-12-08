#include <sem.h>
#include <sem_type.h>
#include <syscall.h>
#include <mutex.h>
#include <cond.h>
#include <thr_internals.h>
#include <linked_list.h>
#include <stddef.h>

// @Author: Armin Zou
// This is semaphore implementation which simply built on the combination of implemenation of mutex
// and conditional variables. With the number of count to manipulate shared resources that is available
// to threads

// Uncomment the following line to disable printf calls
//#define _REMOVE_PRINTF_CALLS_
#ifdef _REMOVE_PRINTF_CALLS_
#define printf(...)
#endif // _REMOVE_PRINTF_CALLS_

int sem_init(sem_t* sem, int count)
{
	// initialize sem by using cond_init
	if(sem == NULL){
		return -1;
	}

	if(count <= 0){
		return -1;
	}

	cond_init(&sem->sem_m);
	sem->count = count;
    return 0;
}

void sem_destroy(sem_t* sem)
{
	assert(sem != NULL);

	// destroy sem by using cond_destroy
	cond_destroy(&sem->sem_m);
	sem->count = -1;
}

void sem_wait(sem_t* sem)
{

	assert(sem != NULL);

	sem->count--;
	if(sem->count < 0){
		// cond wait
		cond_wait(&sem->sem_m, &sem->sem_m.cv);
	}
}

void sem_signal(sem_t* sem)
{

	assert(sem != NULL);
	sem->count++;

	if(sem->count <= 0){
		// cond signal
		cond_signal(&sem->sem_m);
	}

}
