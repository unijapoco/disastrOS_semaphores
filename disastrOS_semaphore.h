#pragma once
#include "linked_list.h"
#include "disastrOS_pcb.h"

#define DSOS_ENOMEM -0xc1400000
#define DSOS_EINVAL -0xc1400001
#define DSOS_ESEMNOFD -0xc1400002
#define DSOS_ENOEX -0xc1400003
#define DSOS_ENOPER -0xc1400004

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
