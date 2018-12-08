#ifndef _SYSCALL_EXT_H
#define _SYSCALL_EXT_H

/* @Author: Armin Zou
 * ADD OTHER LIBSYSCALL-RELATED HEADER CONTENT HERE */

#endif /* _SYSCALL_EXT_H */

void get_initial_esp(void);
int thread_fork(void *esp, void *(*func)(void*), void *arg);
