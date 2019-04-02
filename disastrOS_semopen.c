#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){
	int semnum = running->syscall_args[0];
	if (semnum < 0) {
		running->syscall_retvalue = DSOS_EINVAL;
		return;
	}
	Semaphore *sem = SemaphoreList_byId(&semaphores_list, semnum);
	if (!sem) {
		// new semphore
		sem = Semaphore_alloc(semnum, 1);
		List_insert(&semaphores_list, semaphores_list.last, (ListItem *)sem);
	}
	if (!sem) {
		// semaphore memory allocation failed
		running->syscall_retvalue = DSOS_ENOMEM;
		return;
	}

	SemDescriptor *des = SemdDescriptor_alloc(running->last_fd, sem, running);
	if (!des) {
		running->syscall_retvalue = DSOS_ESEMNOFD;
		return;
	}
	running->last_fd++;
	SemDescriptorPtr *desptr= SemDescriptorPtr_alloc(des);
	List_insert(&running->descriptors, running->descriptors.last, (ListItem *)des);

	des->ptr=desptr;
	List_insert(&sem->descriptors, sem->descriptors.last, (ListItem *)desptr);

	running->syscall_retvalue = semnum;
	return;
}

