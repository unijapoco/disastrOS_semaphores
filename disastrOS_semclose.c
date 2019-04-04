#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){
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

	SemDescriptor *des = (SemDescriptor *)List_detach(&running->sem_descriptors, (ListItem *)desptr->descriptor);

	desptr = (SemDescriptorPtr *)List_detach(&sem->descriptors, (ListItem *)desptr);
	SemDescriptor_free(des);
	SemDescriptorPtr_free(desptr);

	//if the semaphores has no more descriptors, i.e. it's not open in any process we destroy it
	if (!sem->descriptors.size) {
		sem = (Semaphore *)List_detach(&semaphores_list, (ListItem *)sem);
		Semaphore_free(sem);
	}

	running->syscall_retvalue = 0;
	return;
}
