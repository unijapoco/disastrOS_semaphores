#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait(){
	int semnum = running->syscall_args[0];

	Semaphore *sem = SemaphoreList_byId(&semaphores_list, semnum);
	if (!sem) {
		running->syscall_retvalue = DSOS_ENOEX;
		return;
	}

	SemDescriptorPtr *desptr = SemDescriptorPtr_byPID(&sem->descriptors, running->pid);
	if (!desptr) {
		running->syscall_retvalue = DSOS_ENOPER;
		return;
	}

	if (sem->count > 0) {
		sem->count--;
		running->syscall_retvalue = 0;
		return;
	}

	SemDescriptorPtr *waiting_desptr = SemDescriptorPtr_alloc(desptr->descriptor);
	List_insert(&sem->waiting_descriptors, sem->waiting_descriptors.last, (ListItem *)waiting_desptr);
	running->status = Waiting;
	List_insert(&waiting_list, waiting_list.last, (ListItem *)running);
	if (ready_list.first) {
		running = (PCB *)List_detach(&ready_list, ready_list.first);
	} else {
		// you don't end in here
	}
	running->syscall_retvalue = 0;
	return;
}
