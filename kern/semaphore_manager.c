#include <inc/mmu.h>
#include <inc/error.h>
#include <inc/string.h>
#include <inc/assert.h>
#include <inc/environment_definitions.h>

#include <kern/semaphore_manager.h>
#include <kern/memory_manager.h>
#include <kern/sched.h>

void enqueue(struct Env_Queue* queue, struct Env* env);
struct Env* dequeue(struct Env_Queue* queue);

int getSemaphoreNumber(char* name)
{
	int i=0;
	for(; i< nSemaphores; ++i)
	{
		if(strcmp(name, semaphores[i].name)==0)
		{
			return i;
		}
	}
	return -1;
}

int createSemaphore(char* semaphoreName, uint32 initialValue)
{
	panic("This function is not required\n");
	return 0;
}

void waitSemaphore(char* semaphoreName)
{
	panic("This function is not required\n");
}

void signalSemaphore(char* semaphoreName)
{
	panic("This function is not required\n");
}

///// Helper Functions
void enqueue(struct Env_Queue* queue, struct Env* env)
{
	LIST_INSERT_HEAD(queue, env);
}

struct Env* dequeue(struct Env_Queue* queue)
{
	struct Env* envItem = LIST_LAST(queue);
	LIST_REMOVE(queue, envItem);
	return envItem;
}
