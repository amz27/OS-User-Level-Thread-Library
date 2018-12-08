#include <mutex.h>
#include <atomic.h>
#include <syscall.h>
#include <stddef.h>

// @Author: Armin Zou
// This is the implementation of mutex where mutex_init simply initialize the locked variable to 1
// and mutex_lock keep trying to exchange a 0 with locked value atomically.

// Uncomment the following line to disable printf calls
#define _REMOVE_PRINTF_CALLS_
#ifdef _REMOVE_PRINTF_CALLS_
#define printf(...)
#endif // _REMOVE_PRINTF_CALLS_

int mutex_init(mutex_t* mutex)
{
	// initialize the mutex
	if(mutex == NULL){
		return -1;
	}
    mutex->locked = 1;
    return 0;

}

void mutex_destroy(mutex_t* mutex)
{
	// set locked to -1 indicate that is destroyed
	mutex->locked = -1;
}

void mutex_lock(mutex_t* mutex)
{
	// spinlock provides mutual exclusion
    while(!atomic_exchange(&mutex->locked, 0)){
    	yield(-1);
    }
    mutex->locked = 0;
}

void mutex_unlock(mutex_t* mutex)
{
	// spinlock provides mutual exclusion
	atomic_exchange(&mutex->locked, 1);
}
