#ifndef FOS_SEMAPHORE_MANAGER_H
#define FOS_SEMAPHORE_MANAGER_H
#ifndef FOS_KERNEL
# error "This is a FOS kernel header; user programs should not #include it"
#endif

#include <inc/types.h>
#include <inc/queue.h>
#include <inc/trap.h>
#include <inc/memlayout.h>
#include <inc/environment_definitions.h>
#include <kern/sched.h>

LIST_HEAD(Env_Queue, Env);		// Declares 'struct Env_list'

#define MAX_SEMAPHORES 100
int nSemaphores;
struct Semaphore
{
	//queue of all blocked envs on this Semaphore
	struct Env_Queue env_queue;

	//semaphore value
	int value;

	//semaphore name
	char name[256];

};

// Array of all Semaphores
struct Semaphore semaphores[MAX_SEMAPHORES] ;

int getSemaphoreNumber(char* name);
int createSemaphore(char* semaphoreName, uint32 initialValue);
void waitSemaphore(char* semaphoreName);
void signalSemaphore(char* semaphoreName);

#endif /* FOS_SEMAPHORE_MANAGER_H */
