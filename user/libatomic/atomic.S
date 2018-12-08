//@Author: Armin Zou
/* int atomic_exchange(volatile int* obj, int desired); */
    .global atomic_exchange
atomic_exchange:
	mov 4(%esp), %ecx	// obj
	mov 8(%esp), %edx	// desired
	lock xchg %edx, (%ecx)	// atomically exchange
	mov %edx, %eax	// return old val
    ret

/* int atomic_compare_swap(volatile int* obj, int expected, int desired); */
    .global atomic_compare_swap
atomic_compare_swap:
    mov 4(%esp), %edx	// obj
	mov 8(%esp), %eax	// expected
	mov 12(%esp), %ecx	// desired
	lock cmpxchg %ecx, (%edx)	// compare the value of obj with expected value if they are equal, set *obj to desired
    ret

/* int atomic_fetch_add(volatile int* obj, int arg); */
    .global atomic_fetch_add
atomic_fetch_add:
    mov 4(%esp), %ecx	// obj
    mov 8(%esp), %edx	// arg
    lock xadd %edx, (%ecx)	// atomically add
    mov %edx, %eax	// return old val
    ret

/* int atomic_fetch_sub(volatile int* obj, int arg); */
    .global atomic_fetch_sub
atomic_fetch_sub:
    mov 4(%esp), %ecx	// obj
    mov 8(%esp), %edx	// arg
    neg %edx	// negate arg value
    lock xadd %edx, (%ecx)	// atomically add the negated value
    mov %edx, %eax	// return old val
    ret

/* int atomic_load(volatile int* obj); */
    .global atomic_load
atomic_load:
    mov 4(%esp), %ecx	// obj to ecx
    lock cmpxchg %eax, (%ecx)	// since the ZF = 0, then the value of obj is loaded into eax and will be return
    ret

/* void atomic_store(volatile int* obj, int desired); */
    .global atomic_store
atomic_store:
	mov 4(%esp), %ecx	// obj
	mov (%ecx), %eax	// move the value of obj to eax
	mov 8(%esp), %edx	// desired
	lock cmpxchg %edx, (%ecx)	// since ZF = 1, then the desired value is store into the memory location of obj
    ret
