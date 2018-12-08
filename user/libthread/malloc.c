//#include <malloc.h>
#include <stdlib.h>
#include <types.h>
#include <stddef.h>
#include <mutex.h>
#include <thr_internals.h>

// @Author: Armin Zou
// This is simply the thread-safe verision of malloc family where mutex are used to protect the
// memory to threads

// Uncomment the following line to disable printf calls
#define _REMOVE_PRINTF_CALLS_
#ifdef _REMOVE_PRINTF_CALLS_
#define printf(...)
#endif // _REMOVE_PRINTF_CALLS_

void* malloc(size_t size)
{
    /* IMPLEMENT THIS : MAKE THIS THREAD-SAFE */
	mutex_lock(&m_mutex);
	void *bp = _malloc(size);
	mutex_unlock(&m_mutex);

    return bp;
}

void* calloc(size_t nelt, size_t eltsize)
{
    /* IMPLEMENT THIS : MAKE THIS THREAD-SAFE */
    mutex_lock(&m_mutex);
    void *bp = _calloc(nelt, eltsize);
    mutex_unlock(&m_mutex);
	return bp;
}

void* realloc(void* buf, size_t new_size)
{
    /* IMPLEMENT THIS : MAKE THIS THREAD-SAFE */

    mutex_lock(&m_mutex);
    void *bp = _realloc(buf, new_size);
    mutex_unlock(&m_mutex);
    return bp;
}

void free(void* buf)
{
    /* IMPLEMENT THIS : MAKE THIS THREAD-SAFE */
    mutex_lock(&m_mutex);
    _free(buf);
    mutex_unlock(&m_mutex);

}
