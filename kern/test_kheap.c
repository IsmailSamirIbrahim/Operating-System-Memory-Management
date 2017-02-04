#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>
#include <inc/queue.h>

#define Mega  (1024*1024)
#define kilo (1024)

extern int pf_calculate_free_frames() ;
extern uint32 sys_calculate_free_frames() ;
extern int pf_calculate_free_frames();
extern void sys_bypassPageFault(uint8);
extern uint32 sys_rcr2();
extern int execute_command(char *command_string);

extern struct Env_list env_exit_queue;
extern char end_of_kernel[];

struct MyStruct
{
	char a;
	short b;
	int c;
};


int test_kmalloc()
{
	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;

	char *byteArr, *byteArr2 ;
	short *shortArr, *shortArr2 ;
	int *intArr;
	struct MyStruct *structArr ;
	int lastIndexOfByte, lastIndexOfByte2, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	int start_freeFrames = sys_calculate_free_frames() ;

	void* ptr_allocations[20] = {0};
	{
		//Insufficient space
		int freeFrames = sys_calculate_free_frames() ;
		int freeDiskFrames = pf_calculate_free_frames() ;
		uint32 sizeOfKHeap = (KERNEL_HEAP_MAX - KERNEL_HEAP_START + 1) ;
		ptr_allocations[0] = kmalloc(sizeOfKHeap);
		if (ptr_allocations[0] != NULL) panic("Allocating insufficient space: should return NULL");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: pages are not loaded successfully into memory");

		cprintf("kmalloc: current evaluation = 10%");
		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[0] !=  (KERNEL_HEAP_START)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 512) panic("Wrong allocation: pages are not loaded successfully into memory");

		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[1] != (KERNEL_HEAP_START + 2*Mega)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 512) panic("Wrong allocation: pages are not loaded successfully into memory");

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[2] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[2] != (KERNEL_HEAP_START + 4*Mega)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: pages are not loaded successfully into memory");

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[3] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[3] != (KERNEL_HEAP_START + 4*Mega + 4*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: pages are not loaded successfully into memory");

		//7 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[4] = kmalloc(7*kilo);
		if ((uint32) ptr_allocations[4] != (KERNEL_HEAP_START + 4*Mega + 8*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 2) panic("Wrong allocation: pages are not loaded successfully into memory");

		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[5] != (KERNEL_HEAP_START + 4*Mega + 16*kilo) ) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 768) panic("Wrong allocation: pages are not loaded successfully into memory");

		//6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(6*Mega-kilo);
		if ((uint32) ptr_allocations[6] != (KERNEL_HEAP_START + 7*Mega + 16*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1536) panic("Wrong allocation: pages are not loaded successfully into memory");

		//14 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(14*kilo);
		if ((uint32) ptr_allocations[7] != (KERNEL_HEAP_START + 13*Mega + 16*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 4) panic("Wrong allocation: pages are not loaded successfully into memory");

		cprintf("\b\b\b50%");
		//Checking read/write on the allocated spaces
		{
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;

			//Write values
			//In 1st 2 MB
			lastIndexOfByte = (2*Mega-kilo)/sizeof(char) - 1;
			byteArr = (char *) ptr_allocations[0];
			byteArr[0] = minByte ;
			byteArr[lastIndexOfByte] = maxByte ;

			//In 2nd 2 MB
			shortArr = (short *) ptr_allocations[1];
			lastIndexOfShort = (2*Mega-kilo)/sizeof(short) - 1;
			shortArr[0] = minShort;
			shortArr[lastIndexOfShort] = maxShort;

			//In next 2 KB
			intArr = (int *) ptr_allocations[2];
			lastIndexOfInt = (2*kilo)/sizeof(int) - 1;
			intArr[0] = minInt;
			intArr[lastIndexOfInt] = maxInt;

			//In 7 KB
			structArr = (struct MyStruct *) ptr_allocations[4];
			lastIndexOfStruct = (7*kilo)/sizeof(struct MyStruct) - 1;
			structArr[0].a = minByte; structArr[0].b = minShort; structArr[0].c = minInt;
			structArr[lastIndexOfStruct].a = maxByte; structArr[lastIndexOfStruct].b = maxShort; structArr[lastIndexOfStruct].c = maxInt;

			//In 6 MB
			lastIndexOfByte2 = (6*Mega-kilo)/sizeof(char) - 1;
			byteArr2 = (char *) ptr_allocations[6];
			byteArr2[0] = minByte ;
			byteArr2[lastIndexOfByte2 / 2] = maxByte / 2;
			byteArr2[lastIndexOfByte2] = maxByte ;

			//In 14 KB
			shortArr2 = (short *) ptr_allocations[7];
			lastIndexOfShort2 = (14*kilo)/sizeof(short) - 1;
			shortArr2[0] = minShort;
			shortArr2[lastIndexOfShort2] = maxShort;

			//Read values: check that the values are successfully written
			if (byteArr[0] 	!= minByte 	|| byteArr[lastIndexOfByte] 	!= maxByte) panic("Wrong allocation: stored values are wrongly changed!");
			if (shortArr[0] != minShort || shortArr[lastIndexOfShort] 	!= maxShort) panic("Wrong allocation: stored values are wrongly changed!");
			if (intArr[0] 	!= minInt 	|| intArr[lastIndexOfInt] 		!= maxInt) panic("Wrong allocation: stored values are wrongly changed!");

			if (structArr[0].a != minByte 	|| structArr[lastIndexOfStruct].a != maxByte) 	panic("Wrong allocation: stored values are wrongly changed!");
			if (structArr[0].b != minShort 	|| structArr[lastIndexOfStruct].b != maxShort) 	panic("Wrong allocation: stored values are wrongly changed!");
			if (structArr[0].c != minInt 	|| structArr[lastIndexOfStruct].c != maxInt) 	panic("Wrong allocation: stored values are wrongly changed!");

			if (byteArr2[0] != minByte || byteArr2[lastIndexOfByte2/2] != maxByte/2 || byteArr2[lastIndexOfByte2] != maxByte) panic("Wrong allocation: stored values are wrongly changed!");
			if (shortArr2[0] != minShort || shortArr2[lastIndexOfShort2] != maxShort) panic("Wrong allocation: stored values are wrongly changed!");

			if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: pages are not loaded successfully into memory");
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		}

		cprintf("\b\b\b80%");

		//Insufficient space again
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		uint32 restOfKHeap = (KERNEL_HEAP_MAX - KERNEL_HEAP_START + 2*PAGE_SIZE) - (2*Mega+2*Mega+4*kilo+4*kilo+8*kilo+3*Mega+6*Mega+16*kilo) ;
		ptr_allocations[8] = kmalloc(restOfKHeap);
		if (ptr_allocations[8] != NULL) panic("Allocating insufficient space: should return NULL");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: pages are not loaded successfully into memory");

		cprintf("\b\b\b90%");

		//permissions
		uint32 lastAllocAddress = (uint32)ptr_allocations[7] + 16*kilo ;
		uint32 va;
		for (va = KERNEL_HEAP_START; va < lastAllocAddress; va+=PAGE_SIZE)
		{
			unsigned int * table;
			get_page_table(ptr_page_directory, (void*)va, &table);
			uint32 perm = table[PTX(va)] & 0xFFF;
			if ((perm & PERM_USER) == PERM_USER)
				panic("Wrong permissions: pages should be mapped with Supervisor permission only");
		}

		cprintf("\b\b\b100%\n");

	}

	cprintf("Congratulations!! test kmalloc completed successfully.\n");

	return 1;

}

int test_kfree()
{
	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;

	char *byteArr, *byteArr2 ;
	short *shortArr, *shortArr2 ;
	int *intArr;
	struct MyStruct *structArr ;
	int lastIndexOfByte, lastIndexOfByte2, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	int start_freeFrames = sys_calculate_free_frames() ;

	//malloc some spaces
	int i, freeFrames, freeDiskFrames ;
	char* ptr;
	int lastIndices[20] = {0};
	int sums[20] = {0};
	void* ptr_allocations[20] = {0};
	{
		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[0] !=  (KERNEL_HEAP_START)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 512) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[0] = (2*Mega-kilo)/sizeof(char) - 1;

		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[1] != (KERNEL_HEAP_START + 2*Mega)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 512) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[1] = (2*Mega-kilo)/sizeof(char) - 1;

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[2] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[2] != (KERNEL_HEAP_START + 4*Mega)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[2] = (2*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[2];
		for (i = 0; i < lastIndices[2]; ++i)
		{
			ptr[i] = 2 ;
		}

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[3] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[3] != (KERNEL_HEAP_START + 4*Mega + 4*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[3] = (2*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[3];
		for (i = 0; i < lastIndices[3]; ++i)
		{
			ptr[i] = 3 ;
		}

		//7 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[4] = kmalloc(7*kilo);
		if ((uint32) ptr_allocations[4] != (KERNEL_HEAP_START + 4*Mega + 8*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 2) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[4] = (7*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[4];
		for (i = 0; i < lastIndices[4]; ++i)
		{
			ptr[i] = 4 ;
		}

		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[5] != (KERNEL_HEAP_START + 4*Mega + 16*kilo) ) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 768) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[5] = (3*Mega-kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[5];
		for (i = 0; i < lastIndices[5]; ++i)
		{
			ptr[i] = 5 ;
		}

		//6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(6*Mega-kilo);
		if ((uint32) ptr_allocations[6] != (KERNEL_HEAP_START + 7*Mega + 16*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1536) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[6] = (6*Mega-kilo)/sizeof(char) - 1;

		//14 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(14*kilo);
		if ((uint32) ptr_allocations[7] != (KERNEL_HEAP_START + 13*Mega + 16*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 4) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[7] = (14*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[7];
		for (i = 0; i < lastIndices[7]; ++i)
		{
			ptr[i] = 7 ;
		}
	}

	//kfree some of the allocated spaces [15%]
	{
		//kfree 1st 2 MB
		int freeFrames = sys_calculate_free_frames() ;
		int freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[0]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 512 ) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 2nd 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[1]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 512) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[6]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 6*Mega/4096) panic("Wrong kfree: pages in memory are not freed correctly");
	}

	cprintf("kfree: current evaluation = 15%");

	//Check memory access after kfree [15%]
	{
		//2 KB
		ptr = (char*)ptr_allocations[2];
		for (i = 0; i < lastIndices[2]; ++i)
		{
			sums[2] += ptr[i] ;
		}
		if (sums[2] != 2*lastIndices[2])	panic("kfree: invalid read after freeing some allocations");

		//2 KB
		ptr = (char*)ptr_allocations[3];
		for (i = 0; i < lastIndices[3]; ++i)
		{
			sums[3] += ptr[i] ;
		}
		if (sums[3] != 3*lastIndices[3])	panic("kfree: invalid read after freeing some allocations");

		//7 KB
		ptr = (char*)ptr_allocations[4];
		for (i = 0; i < lastIndices[4]; ++i)
		{
			sums[4] += ptr[i] ;
		}
		if (sums[4] != 4*lastIndices[4])	panic("kfree: invalid read after freeing some allocations");

		//3 MB
		ptr = (char*)ptr_allocations[5];
		for (i = 0; i < lastIndices[5]; ++i)
		{
			sums[5] += ptr[i] ;
		}
		if (sums[5] != 5*lastIndices[5])	panic("kfree: invalid read after freeing some allocations");

		//14 KB
		ptr = (char*)ptr_allocations[7];
		for (i = 0; i < lastIndices[7]; ++i)
		{
			sums[7] += ptr[i] ;
		}
		if (sums[7] != 7*lastIndices[7])	panic("kfree: invalid read after freeing some allocations");
	}
	cprintf("\b\b\b30%");

	//Allocate after kfree [15%]
	{
		//20 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[8] = kmalloc(20*kilo);
		if ((uint32) ptr_allocations[8] != (KERNEL_HEAP_START + 13*Mega + 32*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 5) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[8] = (20*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[8];
		for (i = 0; i < lastIndices[8]; ++i)
		{
			ptr[i] = 8 ;
		}

		//1 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[9] = kmalloc(1*Mega);
		if ((uint32) ptr_allocations[9] != (KERNEL_HEAP_START + 13*Mega + 52*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 256) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[9] = (1*Mega)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[9];
		for (i = 0; i < lastIndices[9]; ++i)
		{
			ptr[i] = 9 ;
		}
	}
	cprintf("\b\b\b45%");

	//kfree remaining allocated spaces [15%]
	{
		//kfree 7 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[4]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 2) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[5]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 3*Mega/4096) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 1st 2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[2]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 1) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 2nd 2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[3]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 1) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 14 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[7]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 4) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 20 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[8]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 5) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 1 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[9]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 256) panic("Wrong kfree: pages in memory are not freed correctly");

		if(start_freeFrames != (sys_calculate_free_frames())) {panic("Wrong kfree: not all pages removed correctly at end");}
	}
	cprintf("\b\b\b60%");

	//Check memory access after kfree [15%]
	{
		//Bypass the PAGE FAULT on <MOVB immediate, reg> instruction by setting its length
		//and continue executing the remaining code
		sys_bypassPageFault(3);

		for (i = 0; i < 10; ++i)
		{
			ptr = (char *) ptr_allocations[i];
			ptr[0] = 10;
			//cprintf("\ncr2 = %x, faulted addr = %x", sys_rcr2(), (uint32)&(ptr[0]));
			if (sys_rcr2() != (uint32)&(ptr[0])) panic("kfree: successful access to freed space!! it should not be succeeded");
			ptr[lastIndices[i]] = 10;
			if (sys_rcr2() != (uint32)&(ptr[lastIndices[i]])) panic("kfree: successful access to freed space!! it should not be succeeded");
		}

		//set it to 0 again to cancel the bypassing option
		sys_bypassPageFault(0);
	}
	cprintf("\b\b\b75%");

	//kfree non-exist item [10%]
	{
		//kfree 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[0]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing");

		//kfree 2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[2]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing");

		//kfree 20 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[8]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing");

		//kfree 1 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[9]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing");

	}
	cprintf("\b\b\b85%");

	//check tables	[15%]
	{
		long long va;
		for (va = KERNEL_HEAP_START; va < (long long)KERNEL_HEAP_MAX; va+=PTSIZE)
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, (void*)(uint32)va, &ptr_table);
			if (ptr_table == NULL)
			{
				panic("Wrong kfree: one of the kernel tables is wrongly removed! Tables should not be removed here in kfree");
			}
		}
	}
	cprintf("\b\b\b100%\n");



	cprintf("Congratulations!! test kfree completed successfully.\n");

	return 1;

}

int test_kheap_phys_addr()
{
	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;

	char *byteArr, *byteArr2 ;
	short *shortArr, *shortArr2 ;
	int *intArr;
	struct MyStruct *structArr ;
	int lastIndexOfByte, lastIndexOfByte2, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	int start_freeFrames = sys_calculate_free_frames() ;

	//malloc some spaces
	int i, freeFrames, freeDiskFrames ;
	char* ptr;
	int lastIndices[20] = {0};
	int sums[20] = {0};
	void* ptr_allocations[20] = {0};
	{
		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[0] !=  (KERNEL_HEAP_START)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 512) panic("Wrong allocation: pages are not loaded successfully into memory");

		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[1] != (KERNEL_HEAP_START + 2*Mega)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 512) panic("Wrong allocation: pages are not loaded successfully into memory");

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[2] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[2] != (KERNEL_HEAP_START + 4*Mega)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: pages are not loaded successfully into memory");

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[3] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[3] != (KERNEL_HEAP_START + 4*Mega + 4*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: pages are not loaded successfully into memory");

		//7 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[4] = kmalloc(7*kilo);
		if ((uint32) ptr_allocations[4] != (KERNEL_HEAP_START + 4*Mega + 8*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 2) panic("Wrong allocation: pages are not loaded successfully into memory");

		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[5] != (KERNEL_HEAP_START + 4*Mega + 16*kilo) ) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 768) panic("Wrong allocation: pages are not loaded successfully into memory");

		//6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(6*Mega-kilo);
		if ((uint32) ptr_allocations[6] != (KERNEL_HEAP_START + 7*Mega + 16*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1536) panic("Wrong allocation: pages are not loaded successfully into memory");

		//14 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(14*kilo);
		if ((uint32) ptr_allocations[7] != (KERNEL_HEAP_START + 13*Mega + 16*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 4) panic("Wrong allocation: pages are not loaded successfully into memory");
	}

	//test kheap_physical_address after kmalloc only [40%]
	{
		uint32 va;
		uint32 endVA = KERNEL_HEAP_START + 13*Mega + 32*kilo;
		uint32 allPAs[(13*Mega + 32*kilo)/PAGE_SIZE] ;
		i = 0;
		for (va = KERNEL_HEAP_START; va < endVA; va+=PAGE_SIZE)
		{
			allPAs[i++] = kheap_physical_address(va);
		}
		int ii = i ;
		i = 0;
		int j;
		for (va = KERNEL_HEAP_START; va < endVA; va+=PTSIZE)
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, (void*)va, &ptr_table);
			if (ptr_table == NULL)
				panic("one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed");

			for (j = 0; i < ii && j < 1024; ++j, ++i)
			{
				if ((ptr_table[j] & 0xFFFFF000) != allPAs[i])
				{
					//cprintf("VA = %x, table entry = %x, khep_pa = %x\n",va + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
					panic("Wrong kheap_physical_address");
				}
			}
		}
	}
	cprintf("kheap_physical_address: current evaluation = 40%");

	//kfree some of the allocated spaces
	{
		//kfree 1st 2 MB
		int freeFrames = sys_calculate_free_frames() ;
		int freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[0]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 512 ) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 2nd 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[1]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 512) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[6]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 6*Mega/4096) panic("Wrong kfree: pages in memory are not freed correctly");
	}


	//test kheap_physical_address after kmalloc and kfree [40%]
	{
		uint32 va;
		uint32 endVA = KERNEL_HEAP_START + 13*Mega + 32*kilo;
		uint32 allPAs[(13*Mega + 32*kilo)/PAGE_SIZE] ;
		i = 0;
		for (va = KERNEL_HEAP_START; va < endVA; va+=PAGE_SIZE)
		{
			allPAs[i++] = kheap_physical_address(va);
		}
		int ii = i ;
		i = 0;
		int j;
		for (va = KERNEL_HEAP_START; va < endVA; va+=PTSIZE)
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, (void*)va, &ptr_table);
			if (ptr_table == NULL)
				panic("one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed");

			for (j = 0; i < ii && j < 1024; ++j, ++i)
			{
				if ((ptr_table[j] & 0xFFFFF000) != allPAs[i])
				{
					//cprintf("VA = %x, table entry = %x, khep_pa = %x\n",va + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
					panic("Wrong kheap_physical_address");
				}
			}
		}
	}

	cprintf("\b\b\b80%");

	//test kheap_physical_address on non-mapped area [20%]
	{
		uint32 va;
		uint32 startVA = KERNEL_HEAP_START + 16*Mega;
		i = 0;
		for (va = startVA; va < KERNEL_HEAP_MAX; va+=PAGE_SIZE)
		{
			i++;
		}
		int ii = i ;
		i = 0;
		int j;
		long long va2;
		for (va2 = startVA; va2 < (long long)KERNEL_HEAP_MAX; va2+=PTSIZE)
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, (void*)(uint32)va2, &ptr_table);
			if (ptr_table == NULL)
			{
				panic("one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed");
			}
			for (j = 0; i < ii && j < 1024; ++j, ++i)
			{
				//if ((ptr_table[j] & 0xFFFFF000) != allPAs[i])
				unsigned int page_va = startVA+i*PAGE_SIZE;
				unsigned int supposed_kheap_phys_add = kheap_physical_address(page_va);
				if ((ptr_table[j] & 0xFFFFF000) != supposed_kheap_phys_add)
				{
					//cprintf("VA = %x, table entry = %x, khep_pa = %x\n",va2 + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
					panic("Wrong kheap_physical_address");
				}
			}
		}
	}

	cprintf("\b\b\b100%\n");

	cprintf("Congratulations!! test kheap_physical_address completed successfully.\n");

	return 1;

}

int test_kheap_virt_addr()
{
	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;

	char *byteArr, *byteArr2 ;
	short *shortArr, *shortArr2 ;
	int *intArr;
	struct MyStruct *structArr ;
	int lastIndexOfByte, lastIndexOfByte2, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	int start_freeFrames = sys_calculate_free_frames() ;

	//malloc some spaces
	int i, freeFrames, freeDiskFrames ;
	char* ptr;
	int lastIndices[20] = {0};
	int sums[20] = {0};
	void* ptr_allocations[20] = {0};
	{
		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[0] !=  (KERNEL_HEAP_START)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 512) panic("Wrong allocation: pages are not loaded successfully into memory");

		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[1] != (KERNEL_HEAP_START + 2*Mega)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 512) panic("Wrong allocation: pages are not loaded successfully into memory");

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[2] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[2] != (KERNEL_HEAP_START + 4*Mega)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: pages are not loaded successfully into memory");

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[3] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[3] != (KERNEL_HEAP_START + 4*Mega + 4*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: pages are not loaded successfully into memory");

		//7 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[4] = kmalloc(7*kilo);
		if ((uint32) ptr_allocations[4] != (KERNEL_HEAP_START + 4*Mega + 8*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 2) panic("Wrong allocation: pages are not loaded successfully into memory");

		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[5] != (KERNEL_HEAP_START + 4*Mega + 16*kilo) ) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 768) panic("Wrong allocation: pages are not loaded successfully into memory");

		//6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(6*Mega-kilo);
		if ((uint32) ptr_allocations[6] != (KERNEL_HEAP_START + 7*Mega + 16*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1536) panic("Wrong allocation: pages are not loaded successfully into memory");

		//14 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(14*kilo);
		if ((uint32) ptr_allocations[7] != (KERNEL_HEAP_START + 13*Mega + 16*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc & updating of heap ptr");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 4) panic("Wrong allocation: pages are not loaded successfully into memory");
	}

	uint32 allPAs[(13*Mega + 32*kilo)/PAGE_SIZE] ;
	int numOfFrames = (13*Mega + 32*kilo)/PAGE_SIZE ;

	//test kheap_virtual_address after kmalloc only [40%]
	{
		uint32 va;
		uint32 endVA = KERNEL_HEAP_START + 13*Mega + 32*kilo;
		int i = 0;
		int j;
		for (va = KERNEL_HEAP_START; va < endVA; va+=PTSIZE)
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, (void*)va, &ptr_table);
			if (ptr_table == NULL)
				panic("one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed");

			for (j = 0; i < numOfFrames && j < 1024; ++j, ++i)
			{
				allPAs[i] = (ptr_table[j] & 0xFFFFF000);
				uint32 retrievedVA = kheap_virtual_address(allPAs[i]);

				if (retrievedVA != (va + j*PAGE_SIZE))
				{
					//cprintf("VA = %x, table entry = %x, khep_pa = %x\n",va + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
					panic("Wrong kheap_virtual_address");
				}
			}
		}
	}
	cprintf("kheap_virtual_address: current evaluation = 40%");

	//kfree some of the allocated spaces
	{
		//kfree 1st 2 MB
		int freeFrames = sys_calculate_free_frames() ;
		int freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[0]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 512 ) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 2nd 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[1]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 512) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[6]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 6*Mega/4096) panic("Wrong kfree: pages in memory are not freed correctly");
	}
	//test kheap_virtual_address after kmalloc and kfree [40%]
	{
		uint32 va;
		uint32 endVA = KERNEL_HEAP_START + 13*Mega + 32*kilo;
		int i = 0;
		int j;
		//frames of first 4 MB
		for (i = 0; i < 4*Mega/PAGE_SIZE; ++i)
		{
			uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
			if (retrievedVA != 0)
			{
				panic("Wrong kheap_virtual_address");
			}

		}
		//next frames until 6 MB
		for (i = 4*Mega/PAGE_SIZE; i < (7*Mega + 16*kilo)/PAGE_SIZE; ++i)
		{
			uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
			if (retrievedVA != KERNEL_HEAP_START + i*PAGE_SIZE)
			{
				panic("Wrong kheap_virtual_address");
			}
		}
		//frames of 6 MB
		for (i = (7*Mega + 16*kilo)/PAGE_SIZE; i < (13*Mega + 16*kilo)/PAGE_SIZE; ++i)
		{
			uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
			if (retrievedVA != 0)
			{
				panic("Wrong kheap_virtual_address");
			}
		}
		//frames of last allocation (14 KB)
		for (i = (13*Mega + 16*kilo)/PAGE_SIZE; i < (13*Mega + 32*kilo)/PAGE_SIZE; ++i)
		{
			uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
			if (retrievedVA != KERNEL_HEAP_START + i*PAGE_SIZE)
			{
				panic("Wrong kheap_virtual_address");
			}
		}
	}

	cprintf("\b\b\b80%");

	//test kheap_virtual_address on frames of KERNEL CODE [20%]
	{
		uint32 i;
		for (i = 1*Mega; i < (uint32)(end_of_kernel - KERNEL_BASE); i+=PAGE_SIZE)
		{
			uint32 retrievedVA = kheap_virtual_address(i);
			if (retrievedVA != 0)
			{
				cprintf("PA = %x, retrievedVA = %x\n", i, retrievedVA);
				panic("Wrong kheap_virtual_address");
			}
		}
	}

	cprintf("\b\b\b100%\n");

	cprintf("Congratulations!! test kheap_virtual_address completed successfully.\n");

	return 1;

}

int initFreeFrames;
int initFreeDiskFrames ;
uint8 firstCall = 1 ;
int test_three_creation_functions()
{
	if (firstCall)
	{
		firstCall = 0;
		initFreeFrames = sys_calculate_free_frames() ;
		initFreeDiskFrames = pf_calculate_free_frames() ;
		//Run simple user program
		{
			char command[100] = "run fos_add 4096";
			execute_command(command) ;
		}
	}
	//Ensure that the user directory, page WS and page tables are allocated in KERNEL HEAP
	{
		struct Env * e = NULL;
		struct Env * ptr_env = NULL;
		LIST_FOREACH(ptr_env, &env_exit_queue)
		{
			if (strcmp(ptr_env->prog_name, "fos_add") == 0)
			{
				e = ptr_env ;
				break;
			}
		}
		if (e->pageFaultsCounter != 0)
			panic("Page fault is occur while not expected to. Review the three creation fuctions");

		int pagesInWS = env_page_ws_get_size(e);
		int curFreeFrames = sys_calculate_free_frames() ;
		int curFreeDiskFrames = pf_calculate_free_frames() ;
		//cprintf("diff in page file = %d, pages in WS = %d\n", initFreeDiskFrames - curFreeDiskFrames, pagesInWS);
		if ((initFreeDiskFrames - curFreeDiskFrames) != pagesInWS) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		//cprintf("diff in mem frames = %d, pages in WS = %d\n", initFreeFrames - curFreeFrames, pagesInWS);
		if ((initFreeFrames - curFreeFrames) != 12/*WS*/ + 2*1/*DIR*/ + 2*3/*Tables*/ + 1 /*user WS table*/ + pagesInWS) panic("Wrong allocation: pages are not loaded successfully into memory");

		//allocate 4 KB
		char *ptr = kmalloc(4*kilo);
		if ((uint32) ptr !=  (KERNEL_HEAP_START + (12+2*1+2*3+1)*PAGE_SIZE)) panic("Wrong start address for the allocated space... make sure you create the dir, table and page WS in KERNEL HEAP");
	}

	cprintf("Congratulations!! test the 3 creation functions is completed successfully.\n");

	return 1;
}
