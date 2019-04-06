#pragma once
#include "linked_list.h"
#include "disastrOS_pcb.h"

#define DSOS_ENOMEM -0xFF00
#define DSOS_EINVAL -0xFF01
#define DSOS_ESEMNOFD -0xFF02
#define DSOS_ENOEX -0xFF03
#define DSOS_ENOPER -0xFF04

typedef struct {
  ListItem list;
  int id;
  int count; // counter for semaphore
  ListHead descriptors;
  // this is the list of descriptors that wait
  ListHead waiting_descriptors;
} Semaphore;

void Semaphore_init();

Semaphore* Semaphore_alloc(int id, int type);
int Semaphore_free(Semaphore* semaphore);

typedef ListHead SemaphoreList;

Semaphore* SemaphoreList_byId(SemaphoreList* l, int id);

void SemaphoreList_print(ListHead* l);
