#include <syscall_int.h>
// @Author: Armin Zou
/* int fork(void); */
    .global fork
fork:
    int $FORK_INT
    ret

/* int exec(char* execname, char* argvec[]); */
    .global exec
exec:
    //leal 0x8(%esp), %esi
    push %esi
    mov %esp, %esi
    add $8, %esi
    int $EXEC_INT
    pop %esi
    ret

/* void set_status(int status); */
    .global set_status
set_status:
    push %esi
    mov 8(%esp), %esi
    int $SET_STATUS_INT
    pop %esi
    ret

/* void vanish(void) NORETURN; */
    .global vanish
vanish:
    int $VANISH_INT
vanish_loop: /* infinite loop for now */
        nop
    jmp vanish_loop

/* int wait(int* status_ptr); */
    .global wait
wait:
	push %esi
	mov 0x8(%esp), %esi
	int $WAIT_INT
	pop %esi
    ret

/* void task_vanish(int status) NORETURN; */
    .global task_vanish
task_vanish:
	push %esi
    mov 0x8(%esp), %esi
    int $TASK_VANISH_INT
    pop %esi
task_vanish_loop: /* infinite loop for now */
        nop
    jmp task_vanish_loop

/* int gettid(void); */
    .global gettid
gettid:
    push %esi
    mov %esp, %esi
    add $8, %esi
    int $GETTID_INT
    pop %esi
    ret

/* int yield(int pid); */
    .global yield
yield:
    push %esi
    mov 0x8(%esp), %esi
    int $YIELD_INT
    pop %esi
    ret

/* int deschedule(int* flag); */
    .global deschedule
deschedule:
    push %esi
    mov 0x8(%esp), %esi
    int $DESCHEDULE_INT
    pop %esi
    ret

/* int make_runnable(int pid); */
    .global make_runnable
make_runnable:
    push %esi
    mov 0x8(%esp), %esi
    int $MAKE_RUNNABLE_INT
    pop %esi
    ret

/* unsigned int get_ticks(void); */
    .global get_ticks
get_ticks:
    /* IMPLEMENT THIS */
    int $GET_TICKS_INT
    ret

/* int sleep(int ticks); */
    .global sleep
sleep:
    push %esi
    mov 0x8(%esp), %esi
    int $SLEEP_INT
    pop %esi
    ret

/* int new_pages(void* addr, int len); */
    .global new_pages
new_pages:
    push %esi
    mov %esp, %esi
    add $8, %esi
    int $NEW_PAGES_INT
    pop %esi
    ret

/* int remove_pages(void* addr); */
    .global remove_pages
remove_pages:
    push %esi
    mov 8(%esp), %esi
    int $REMOVE_PAGES_INT
    pop %esi
    ret

/* int getchar(void); */
    .global getchar
getchar:
    int $GETCHAR_INT
    ret

/* int readline(int size, char* buf); */
    .global readline
readline:
    push %esi
    mov %esp, %esi
    add $8, %esi
    int $READLINE_INT
    pop %esi
    ret

/* int print(int size, char* buf); */
    .global print
print:
    /* IMPLEMENT THIS */
    push %esi
    mov %esp, %esi
    add $8, %esi
    int $PRINT_INT
    pop %esi
    ret

/* int set_term_color(int color); */
    .global set_term_color
set_term_color:
    push %esi
    mov 0x8(%esp), %esi
    int $SET_TERM_COLOR_INT
    pop %esi
    ret

/* int set_cursor_pos(int row, int col); */
    .global set_cursor_pos
set_cursor_pos:
    push %esi
    mov %esp, %esi
    add $8, %esi
    int $SET_CURSOR_POS_INT
    pop %esi
    ret

/* int get_cursor_pos(int* row, int* col); */
    .global get_cursor_pos
get_cursor_pos:
    //leal 0x8(%esp), %esi
    push %esi
    mov %esp, %esi
    add $8, %esi
    int $GET_CURSOR_POS_INT
    pop %esi
    ret

/* void halt(); */
    .global halt
halt:
    int $HALT_INT
    ret

/* int readfile(char* filename, char* buf, int count, int offset); */
    .global readfile
readfile:
    push %esi
    mov %esp, %esi
    add $8, %esi
    int $READFILE_INT
    pop %esi
    ret

/* void misbehave(int mode); */
    .global misbehave
misbehave:
    push %esi
    mov 0x8(%esp), %esi
    int $MISBEHAVE_INT
    pop %esi
    ret

/* int swexn(void* esp3, swexn_handler_t eip, void* arg, ureg_t* newureg); */
    .global swexn
swexn:
    push %esi
    mov %esp, %esi
    add $8, %esi
    int $SWEXN_INT
    pop %esi
    ret

/* get_esp(void)*/
    .globl get_initial_esp
get_initial_esp:
    movl %esp, %eax // Get current esp
    add $4, %eax    // Account for pushed return addr
    ret

/* int thread_fork(void *esp, void *(*func)(void*), void *arg); */
	.globl thread_fork
thread_fork:
    push %ebx
	mov 0x8(%esp), %ebx	// esp
    mov 0xc(%esp), %ecx // func
    mov 0x10(%esp), %edx // arg
    int $THREAD_FORK_INT
    cmp $0, %eax
    jne parent
    movl %ebx, %ebp
    movl %ebx, %esp
    push %edx	// push arg
    call %ecx	// call func on arg
    push %eax
    call thr_exit
    parent:
    pop %ebx
    ret

