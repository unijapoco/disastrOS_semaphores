#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#include "disastrOS.h"


// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}

void sem_test1(void *args) {
	printf("1: Testing the various semaphore features\n");
	printf("1: Creating a new semaphore with id: 1234\n");
	int semnum = disastrOS_semOpen(1234);
	printf("1: Return value: %d\n", semnum);
  disastrOS_printStatus();
	disastrOS_preempt();

	printf("1: Trying to create a semaphore with a negative id\n");
	int error_semnum = disastrOS_semOpen(-1234);
  disastrOS_printStatus();
	printf("1: Function returned error value: %d\n", error_semnum);

	printf("1: Closing the semaphore 1234\n");
	int res = disastrOS_semClose(1234);
	printf("1: Return value: %d\n", res);
  disastrOS_printStatus();
	disastrOS_preempt();

	printf("1: Creating a new semaphore with id: 4321\n");
	semnum = disastrOS_semOpen(4321);
  disastrOS_printStatus();
	disastrOS_preempt();

	printf("1: Simulating a critical section\n");
	res = disastrOS_semWait(4321);
	printf("1: INSIDE THE CRITICAL SECTION\n");
  disastrOS_printStatus();
	printf("1: going to sleep\n");
	disastrOS_sleep(10);
	printf("1: FINISHING CRITICAL SECTION\n");
  disastrOS_printStatus();
	res = disastrOS_semPost(4321);
	printf("1: semPost return value: %d\n", res);
  disastrOS_printStatus();
	printf("1: Cleaning up now\n");
	res = disastrOS_semClose(4321);
	printf("1: semClose return value %d", res);
  disastrOS_printStatus();

  disastrOS_exit(disastrOS_getpid()+1);
}

void sem_test2(void *args) {
	printf("2: Opening the semaphore my brother created\n");
	int semnum = disastrOS_semOpen(1234);
	printf("2: Return value: %d\n", semnum);
  disastrOS_printStatus();
  disastrOS_printStatus();
	disastrOS_preempt();

	printf("2: Closing the semaphore 1234 (that my brother created)\n");
	int res = disastrOS_semClose(1234);
	printf("2: Return value: %d\n", res);
  disastrOS_printStatus();
	printf("2: Notice the semaphore was also destroyed, because it had no more open descriptors\n");
	disastrOS_preempt();

	printf("2: Trying to close a semaphore I don't own\n");
	res = disastrOS_semClose(4321);
	printf("2: Return value: %d\n", res);
  disastrOS_printStatus();

	printf("2: Trying to wait on a semaphore I don't own\n");
	res = disastrOS_semWait(4321);
	printf("2: Return value: %d\n", res);
  disastrOS_printStatus();

	printf("2: Trying to post a semaphore I don't own\n");
	res = disastrOS_semPost(4321);
	printf("2: Return value: %d\n", res);
  disastrOS_printStatus();

	semnum = disastrOS_semOpen(4321);
  disastrOS_printStatus();

	disastrOS_preempt();

	printf("2: Waiting on my brother's semaphore, you won't hear back from me until my brother posts it\n");
	res = disastrOS_semWait(4321);
	printf("2: semwait return value: %d\n", res);
	printf("2: Entering my critical section immediately\n");
	res = disastrOS_semPost(4321);

	res = disastrOS_semClose(4321);
	printf("2: semclose return value: %d\n", res);
  disastrOS_printStatus();
	disastrOS_preempt();

  disastrOS_exit(disastrOS_getpid()+1);
}

void my_init(void *args) {
  disastrOS_spawn(sleeperFunction, 0);
	disastrOS_printStatus();
	disastrOS_spawn(sem_test1, 0);
	disastrOS_spawn(sem_test2, 0);
	int retval;
	disastrOS_wait(0, &retval);
	disastrOS_wait(0, &retval);
	disastrOS_shutdown();
}

int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue
  printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(my_init, 0, logfilename);
  return 0;
}
