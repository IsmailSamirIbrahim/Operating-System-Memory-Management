#ifndef FOS_SHARED_MEMORY_MANAGER_H
#define FOS_SHARED_MEMORY_MANAGER_H
#ifndef FOS_KERNEL
# error "This is a FOS kernel header; user programs should not #include it"
#endif

//#include <inc/types.h>
//#include <inc/queue.h>
//#include <inc/trap.h>
//#include <inc/memlayout.h>
#include <inc/environment_definitions.h>

//number of shared object
int nShares;

struct Share
{
	//share size
	int size;
	//share name
	char name[256];
	//references, number of envs looking at this shared mem object
	uint32 references;
	//sharing permissions (0: ReadOnly, 1:Writable)
	uint8 isWritable;
	//to store frames to be shared
	uint32 framesStorage[1024];
};

//array of all shared objects
struct Share shares[MAX_SHARES] ;

int getShareNumber(char* name);

int createSharedObject(char* shareName, uint32 size, uint8 isWritable, void** returned_shared_address);
int getSharedObject(char* shareName, void** returned_shared_address);
int freeSharedObject(char* shareName);

#endif /* FOS_SHARED_MEMORY_MANAGER_H */
