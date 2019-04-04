#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost(){
	int semnum = running->syscall_args[0];

	Semaphore *sem = SemaphoreList_byId(&semaphores_list, semnum);
	if (!sem) {
		running->syscall_retvalue = DSOS_ENOEX;
		return;
	}

	SemDescriptorPtr *desptr = SemDescriptorPtr_byPID(&sem->descriptors, running->pid);
	if (!sem) {
		running->syscall_retvalue = DSOS_ENOPER;
		return;
	}

	sem->count++;
	if (sem->count == (0 + 1) && sem->waiting_descriptors.first) {
		List_detach(&sem->waiting_descriptors, (ListItem *)desptr);
		PCB *next_proc = desptr->descriptor->pcb;
		List_detach(&waiting_list, (ListItem *)next_proc);

		running->status = Ready;
		List_insert(&ready_list, ready_list.last, (ListItem *)running);

		next_proc->status = Running;
		running = next_proc;
	}
	running->syscall_retvalue = 0;
	return;
}
