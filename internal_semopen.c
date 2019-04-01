#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){
	int semnum = running->syscall_args[0];
	Semaphore *sem = Semaphore_alloc(semnum, 1);
	if (!sem)
		running->syscall_retvalue = DSOS_ENOMEM;
	// Need to check for errors here v
	List_insert(semaphoress_list, semaphors_list->last, sem);
	running->syscall_retvalue = semnum;
	return;
}
