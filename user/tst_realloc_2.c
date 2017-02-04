/* *********************************************************** */
/* PAGE_WS_MAX_SIZE = 3000 */
/* ALLOCATION STRATEGY = FIRST FIT */
/* *********************************************************** */
#include <inc/lib.h>

void _main(void)
{
	int envID = sys_getenvid();
	volatile struct Env* myEnv;
	myEnv = &(envs[envID]);

	int Mega = 1024*1024;
	int kilo = 1024;
	void* ptr_allocations[20] = {0};
	int freeFrames ;
	//[0] Make sure there're available places in the WS
	int w = 0 ;
	int requiredNumOfEmptyWSLocs = 1050;
	int numOfEmptyWSLocs = 0;
	for (w = 0 ; w < myEnv->page_WS_max_size ; w++)
	{
		if( myEnv->__uptr_pws[w].empty == 1)
			numOfEmptyWSLocs++;
	}
	if (numOfEmptyWSLocs < requiredNumOfEmptyWSLocs)
		panic("Insufficient number of WS empty locations! please increase the PAGE_WS_MAX_SIZE");

	cprintf("This test has FIVE cases. A pass message will be displayed after each one.\n");

	int usedDiskPages;

	//[1] Allocate all
	{
		usedDiskPages = sys_pf_calculate_allocated_pages();

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		ptr_allocations[0] = malloc(1*Mega-kilo);
		if ((uint32) ptr_allocations[0] != (USER_HEAP_START)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 256+1 ) panic("Wrong allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 1 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 256) panic("Extra or less pages are allocated in PageFile");

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[1] = malloc(1*Mega-kilo);
		if ((uint32) ptr_allocations[1] != (USER_HEAP_START + 1*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 256 ) panic("Wrong allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 256) panic("Extra or less pages are allocated in PageFile");

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[2] = malloc(1*Mega-kilo);
		if ((uint32) ptr_allocations[2] != (USER_HEAP_START + 2*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 256 ) panic("Wrong allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 256) panic("Extra or less pages are allocated in PageFile");

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[3] = malloc(1*Mega-kilo);
		if ((uint32) ptr_allocations[3] != (USER_HEAP_START + 3*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 256 ) panic("Wrong allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 256) panic("Extra or less pages are allocated in PageFile");

		//Allocate 2 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[4] = malloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[4] != (USER_HEAP_START + 4*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 512 + 1) panic("Wrong allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 1 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 512) panic("Extra or less pages are allocated in PageFile");

		//Allocate 2 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[5] = malloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[5] != (USER_HEAP_START + 6*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 512) panic("Wrong allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 512) panic("Extra or less pages are allocated in PageFile");

		//Allocate 3 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[6] = malloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[6] != (USER_HEAP_START + 8*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 768 + 1) panic("Wrong allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 1 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 768) panic("Extra or less pages are allocated in PageFile");

		//Allocate 3 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[7] = malloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[7] != (USER_HEAP_START + 11*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 768 + 1) panic("Wrong allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 1 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 768) panic("Extra or less pages are allocated in PageFile");
	}

	//[2] Free some to create holes
	{
		//1 MB Hole
		usedDiskPages = sys_pf_calculate_allocated_pages();
		freeFrames = sys_calculate_free_frames() ;
		free(ptr_allocations[1]);
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong free: table is not removed correctly");
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 256) panic("Wrong free: Extra or less pages are removed from PageFile");

		//2 MB Hole
		usedDiskPages = sys_pf_calculate_allocated_pages();
		freeFrames = sys_calculate_free_frames() ;
		free(ptr_allocations[4]);
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong free: table is not removed correctly");
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 512) panic("Wrong free: Extra or less pages are removed from PageFile");

		usedDiskPages = sys_pf_calculate_allocated_pages();
		//3 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		free(ptr_allocations[6]);
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong free: table is not removed correctly");
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 768) panic("Wrong free: Extra or less pages are removed from PageFile");
	}

	int cnt = 0;

	//Bypass the PAGE FAULT on <MOVB immediate, reg> instruction by setting its length
	//and continue executing the remaining code
	sys_bypassPageFault(3);

	//[3] Test Re-allocation
	{
		/*CASE1: Re-allocate with size  = 0*/

		char *byteArr = (char *) ptr_allocations[0];

		//Reallocate with size = 0 [delete it]
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		ptr_allocations[0] = realloc(ptr_allocations[0], 0);

		//[1] test return address & re-allocated space
		if ((uint32) ptr_allocations[0] != 0) panic("Wrong start address for the re-allocated space...it should return NULL!");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong re-allocation");
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 256) panic("Wrong re-allocation to size 0: Extra or less pages are removed from PageFile");

		//[2] test memory access
		byteArr[0] = 10;
		if (sys_rcr2() != (uint32)&(byteArr[0])) panic("successful access to re-allocated space with size 0!! it should not be succeeded");
		byteArr[(1*Mega-kilo)/sizeof(char) - 1] = 10;
		if (sys_rcr2() != (uint32)&(byteArr[(1*Mega-kilo)/sizeof(char) - 1])) panic("successful access to reallocated space of size 0!! it should not be succeeded");

		cprintf("\nCASE1: (Re-allocate with size  = 0) is succeeded...\n") ;

		//set it to 0 again to cancel the bypassing option
		sys_bypassPageFault(0);

		/*CASE2: Re-allocate with address = NULL*/

		//new allocation with size = 2.5 MB, should be placed in 3rd hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		ptr_allocations[10] = realloc(NULL, 2*Mega + 510*kilo);

		if ((uint32) ptr_allocations[10] != (USER_HEAP_START + 8*Mega)) panic("Wrong start address for the allocated space... ");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong re-allocation");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 640) panic("Extra or less pages are allocated in PageFile");

		//Fill it with data
		freeFrames = sys_calculate_free_frames() ;
		int *intArr = (int*) ptr_allocations[10];
		int lastIndexOfInt1 = (2*Mega + 510*kilo)/sizeof(int) - 1;

		int i = 0;
		for (i=0; i <= lastIndexOfInt1 ; i++)
		{
			intArr[i] = i ;
		}

		if ((freeFrames - sys_calculate_free_frames()) != 640 + 1) panic("Wrong placement when accessing the allocated space");

		//[2] test memory access
		freeFrames = sys_calculate_free_frames() ;
		for (i=0; i <= lastIndexOfInt1 ; i++)
		{
			cnt++;
			if (intArr[i] != i) panic("Wrong re-allocation: stored values are wrongly changed!");
		}
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong placement when accessing the allocated space");

		cprintf("CASE2: (Re-allocate with address = NULL) is succeeded...\n") ;

		/*CASE3: Re-allocate in the existing internal fragment (no additional pages are required)*/

		//Reallocate last allocation with 1 extra KB [should be placed in the existing 2 KB internal fragment]
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		ptr_allocations[10] = realloc(ptr_allocations[10], 2*Mega + 510*kilo + kilo);

		//[1] test return address & re-allocated space
		if ((uint32) ptr_allocations[10] != (USER_HEAP_START + 8*Mega)) panic("Wrong start address for the re-allocated space... ");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong re-allocation");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 0) panic("Extra or less pages are allocated in PageFile");

		//[2] test memory access
		freeFrames = sys_calculate_free_frames() ;
		int lastIndexOfInt2 = (2*Mega + 510*kilo + kilo)/sizeof(int) - 1;
		for (i=lastIndexOfInt1 + 1; i < lastIndexOfInt2 ; i++)
		{
			intArr[i] = i ;
		}
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong placement when accessing the allocated space");

		freeFrames = sys_calculate_free_frames() ;
		for (i=0; i < lastIndexOfInt2 ; i++)
		{
			cnt++;
			if (intArr[i] != i) panic("Wrong re-allocation: stored values are wrongly changed!");
		}
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong placement when accessing the allocated space");

		//[3] test freeing it after expansion
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		free(ptr_allocations[10]);

		if ((sys_calculate_free_frames() - freeFrames) != 640+1) panic("Wrong free of the re-allocated space");
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 640) panic("Wrong free: Extra or less pages are removed from PageFile");

		cprintf("CASE3: (Re-allocate in the existing internal fragment) is succeeded...\n") ;

		/*CASE4: Re-allocate that can NOT fit in any free fragment*/

		//Fill 3rd allocation with data
		freeFrames = sys_calculate_free_frames() ;
		intArr = (int*) ptr_allocations[2];
		lastIndexOfInt1 = (1*Mega)/sizeof(int) - 1;

		i = 0;
		for (i=0; i <= lastIndexOfInt1 ; i++)
		{
			intArr[i] = i ;
		}
		if ((freeFrames - sys_calculate_free_frames()) != 256+1) panic("Wrong re-allocation");

		//Reallocate it to large size that can't be fit in any free segment
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		ptr_allocations[2] = realloc(ptr_allocations[2], (USER_HEAP_MAX - USER_HEAP_START - 13*Mega));

		//cprintf("%x\n", ptr_allocations[2]);

		//[1] test return address & re-allocated space
		if ((uint32) ptr_allocations[2] != (USER_HEAP_START + 2*Mega)) panic("Wrong start address for the re-allocated space... ");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong re-allocation");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 0) panic("Extra or less pages are allocated in PageFile");

		//[2] test memory access
		freeFrames = sys_calculate_free_frames() ;
		for (i=0; i <= lastIndexOfInt1 ; i++)
		{
			cnt++;
			if (intArr[i] != i) panic("Wrong re-allocation: stored values are wrongly changed!");
		}
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong placement when accessing the allocated space");

		//[3] test freeing it after expansion
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		free(ptr_allocations[2]);

		if ((sys_calculate_free_frames() - freeFrames) != 256+1) panic("Wrong free of the re-allocated space");
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 256) panic("Wrong free: Extra or less pages are removed from PageFile");

		cprintf("CASE4: (Re-allocate that can NOT fit in any free fragment) is succeeded...\n") ;

		/*CASE5: Re-allocate that only fit in the 1st or last segment*/

		//[1] create 4 MB hole at beginning of the heap
		//Take 2 MB from currently 3 MB hole at beginning of the heap
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		ptr_allocations[10] = malloc(2*Mega-kilo);

		if ((uint32) ptr_allocations[10] != (USER_HEAP_START)) panic("Wrong start address for the allocated space... ");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: ");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 512) panic("Extra or less pages are allocated in PageFile");

		//remove 1 MB allocation between 1 MB hole and 2 MB hole to create 4 MB hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		free(ptr_allocations[3]);

		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong free: ");
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 256) panic("Wrong free: Extra or less pages are removed from PageFile");

		//[2] break down the remaining of the heap into 3 MB segments
		/*for (i = USER_HEAP_START + 14*Mega ; i < USER_HEAP_MAX; i += 3*Mega)
		{
			//allocate 1 page after each 3 MB
			sys_allocateMem(i, PAGE_SIZE) ;
		}*/

		malloc(5*Mega-kilo);

		//Fill last 3MB allocation with data
		freeFrames = sys_calculate_free_frames() ;
		intArr = (int*) ptr_allocations[7];
		lastIndexOfInt1 = (3*Mega)/sizeof(int) - 1;

		i = 0;
		for (i=0; i <= lastIndexOfInt1 ; i++)
		{
			intArr[i] = i ;
		}
		if ((freeFrames - sys_calculate_free_frames()) != 768 + 2) panic("Wrong placement when accessing the allocated space");

		//Reallocate it to 4 MB, so that it can either fit at the 1st fragment or last one
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		ptr_allocations[7] = realloc(ptr_allocations[7], 4*Mega-kilo);

		//[1] test return address & re-allocated space
		if ((uint32) ptr_allocations[7] != (USER_HEAP_START + 2*Mega) && (uint32) ptr_allocations[7] != (USER_HEAP_START + 19*Mega)) panic("Wrong start address for the re-allocated space... ");
		if ((uint32) ptr_allocations[7] == (USER_HEAP_START + 2*Mega))
		{
			if ((sys_calculate_free_frames() - freeFrames) != 768 + 2) panic("Wrong re-allocation: either extra pages are allocated in memory or pages not removed correctly when moving the reallocated block");
		}
		else if ((uint32) ptr_allocations[7] == (USER_HEAP_START + 19*Mega))
		{
			if ((sys_calculate_free_frames() - freeFrames) != 768 + 2 - 1) panic("Wrong re-allocation: either extra pages are allocated in memory or pages not removed correctly when moving the reallocated block");
		}
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 256) panic("Extra or less pages are allocated in PageFile");

		//[2] test memory access
		freeFrames = sys_calculate_free_frames() ;
		lastIndexOfInt2 = (4*Mega)/sizeof(int) - 1;
		intArr = (int*) ptr_allocations[7];
		for (i=lastIndexOfInt1+1; i < lastIndexOfInt2 ; i++)
		{
			intArr[i] = i ;
		}

		if ((freeFrames - sys_calculate_free_frames()) != 256 + 1) panic("Wrong placement when accessing the allocated space");

		freeFrames = sys_calculate_free_frames() ;
		for (i=0; i < lastIndexOfInt2 ; i++)
		{
			cnt++;
			if (intArr[i] != i) panic("Wrong re-allocation: stored values are wrongly changed!");
		}
		if ((freeFrames - sys_calculate_free_frames()) != 768 + 1) panic("Wrong placement when accessing the allocated space");

		//[3] test freeing it after expansion
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		free(ptr_allocations[7]);

		if ((sys_calculate_free_frames() - freeFrames) != 1024+2) panic("Wrong free of the re-allocated space");
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 1024) panic("Wrong free: Extra or less pages are removed from PageFile");

		cprintf("CASE5: (Re-allocate that only fit in the 1st or last segment) is succeeded...\n") ;

	}


	cprintf("Congratulations!! test realloc [2] completed successfully.\n");

	return;
}
