#include <inc/mmu.h>
#include <inc/error.h>
#include <inc/string.h>
#include <inc/assert.h>
#include <inc/environment_definitions.h>

#include <kern/shared_memory_manager.h>
#include <kern/memory_manager.h>
#include <kern/syscall.h>

int getShareNumber(char* name)
{
	int i=0;
	for(; i< nShares; ++i)
	{
		if(strcmp(name, shares[i].name)==0)
		{
			return i;
		}
	}
	return -1;
}

int createSharedObject(char* shareName, uint32 size, uint8 isWritable, void** returned_shared_address)
{
	// your code is here, remove the panic and write your code
	//panic("createSharedObject() is not implemented yet...!!");
	struct Env* myenv = curenv; //The calling environment

	// This function should create the shared object with the given size
	// in the SHARED area of the current environment
	// and return the start address of it by setting the "*returned_shared_address"
	// RETURN:
	//	a) 0 if success
	//	b) E_NO_MEM if the required size exceed the max shared area "USER_SHARED_MEM_MAX"
	//	c) E_SHARED_MEM_EXISTS if the shared object already exists
	//	d) E_NO_SHARE if the number of shared objects reaches max "MAX_SHARES"

	// Steps:
	//	1) Allocate the required space in the physical memory on a PAGE boundary
	//	2) Map the allocated space on the "shared_free_address" of current environment "curenv": object OWNER, with writable permissions
	//	3) Update the "shared_free_address" of the Current environment to be aligned on a PAGE boundary (i.e. 4 KB boundary)
	//	4) Add all allocated frames to "frames_storage" of this shared object to keep track of them for later use
	//		(use: add_frame_to_storage())
	//	5) Add the shared object to the end of the "shares" array (use "nShares"), remember to:
	//		a) Set the data members of the object with suitable values (name, size, ...)
	//		b) Set references to 1 (as there's 1 user environment that use the object now - OWNER)
	//		c) Store the object's isWritable flag (0:ReadOnly, 1:Writable) for later use by getSharedObject()
	// 	6) Return the start address of the object by setting the "*returned_shared_address"
	panic("This function is not required\n");
	return 0;
}

int getSharedObject(char* shareName, void** returned_shared_address)
{
	// your code is here, remove the panic and write your code
	//panic("getSharedObject() is not implemented yet...!!");

	struct Env* myenv = curenv; //The calling environment

	// This function should share the required object in SHARED area of the current environment (with the specified permissions: read_only/writable)
	// and return the start address of it by setting the "*returned_shared_address"
	// RETURN:
	//	a) 0 if success
	//	b) E_SHARED_MEM_NOT_EXISTS if the shared object is not exists
	//	c) E_NO_MEM if the required size exceed the max shared area "USER_SHARED_MEM_MAX"


	// Steps:
	//	1) Get the shared object from the "shares" array
	//	2) Get its physical frames from the frames_storage
	//		(use: get_frame_from_storage())
	//	3) Share these frames with the current environment "curenv" starting from its "shared_free_address"
	//  4) make sure that read-only object must be shared "read only", use the flag isWritable to make it either read-only or writable
	//	5) Update the "shared_free_address" of the current environment to be aligned on a PAGE boundary
	//	6) Update references
	// 	7) Return the start address of the shared object by setting the "*returned_shared_address"
	panic("This function is not required\n");
	return 0;
}

//========================== BONUS ============================
// Free Shared Object

int freeSharedObject(char* shareName)
{
	struct Env* myenv = curenv; //The calling environment
	// your code is here, remove the panic and write your code
	//panic("freeSharedObject() is not implemented yet...!!");

	// This function should free (delete) the shared object from the SHARED area of the current environment
	// If this is the last shared env, then the "frames_store" should be cleared and the shared object should be deleted
	// RETURN:
	//	a) 0 if success
	//	b) E_SHARED_MEM_NOT_EXISTS if the shared object is not exists


	// Steps:
	//	1) Get the shared object from the "shares" array
	//	2) Find its start address in the current environment
	//	2) Unmap it from the current environment "curenv"
	//	3) If one or more table becomes empty, remove it
	//	4) Update references
	//	5) If this is the last share:
	//		a) clear the frames_storage of this shared object (use: clear_frames_storage())
	//		b) clear all other data members of this object (name, size, ...)
	//	6) Flush the cache "tlbflush()"
	panic("This function is not required\n");
	return 0;
}
