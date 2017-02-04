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
	int usedDiskPages;
	//[0] Make sure there're available places in the WS (1030)
	int w = 0 ;
	int requiredNumOfEmptyWSLocs = 1030;
	int numOfEmptyWSLocs = 0;
	for (w = 0 ; w < myEnv->page_WS_max_size; w++)
	{
		if( myEnv->__uptr_pws[w].empty == 1)
			numOfEmptyWSLocs++;
	}
	if (numOfEmptyWSLocs < requiredNumOfEmptyWSLocs)
		panic("Insufficient number of WS empty locations! please increase the PAGE_WS_MAX_SIZE");

	cprintf("This test has THREE cases. A pass message will be displayed after each one.\n");

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

		usedDiskPages = sys_pf_calculate_allocated_pages();
		//2 MB Hole
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

	//[3] Test Re-allocation
	{
		/*CASE1: Re-allocate that's fit in the same location*/

		//Allocate 512 KB - should be placed in 1st hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[8] = malloc(512*kilo - kilo);
		if ((uint32) ptr_allocations[8] != (USER_HEAP_START + 1*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 128) panic("Wrong allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 128) panic("Extra or less pages are allocated in PageFile");

		//Fill it with data
		freeFrames = sys_calculate_free_frames() ;
		int *intArr = (int*) ptr_allocations[8];
		int lastIndexOfInt1 = ((512)*kilo)/sizeof(int) - 1;

		int i = 0;
		for (i=0; i <= lastIndexOfInt1 ; i++)
		{
			intArr[i] = i ;
		}
		if ((freeFrames - sys_calculate_free_frames()) != 128 + 1 ) panic("Wrong placement when accessing the allocated space");

		//Reallocate it [expanded in the same place]
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		ptr_allocations[8] = realloc(ptr_allocations[8], 512*kilo + 256*kilo - kilo);

		//[1] test return address & re-allocated space
		if ((uint32) ptr_allocations[8] !=(USER_HEAP_START + 1*Mega)) panic("Wrong start address for the re-allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 64) panic("Wrong re-allocation");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong re-allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 64) panic("Extra or less pages are allocated in PageFile");

		//[2] test memory access
		freeFrames = sys_calculate_free_frames() ;
		int lastIndexOfInt2 = ((512+256)*kilo)/sizeof(int) - 1;

		for (i=lastIndexOfInt1+1; i < lastIndexOfInt2 ; i++)
		{
			intArr[i] = i ;
		}
		if ((freeFrames - sys_calculate_free_frames()) != 64) panic("Wrong placement when accessing the allocated space");

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
		free(ptr_allocations[8]);
		//if ((sys_calculate_free_frames() - freeFrames) != 192) panic("Wrong free of the re-allocated space");
		if ((sys_calculate_free_frames() - freeFrames) != 128+64+1) panic("Wrong re-allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 192) panic("Extra or less pages are allocated in PageFile");

		cprintf("\nCASE1: (Re-allocate that's fit in the same location) is succeeded...\n") ;

		/*CASE2: Re-allocate that's not fit in the same location*/

		//Allocate 1.5 MB - should be placed in 2nd hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		ptr_allocations[8] = malloc(1*Mega + 512*kilo - kilo);

		if ((uint32) ptr_allocations[8] != (USER_HEAP_START + 4*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 384) panic("Wrong allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong re-allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 384) panic("Extra or less pages are allocated in PageFile");

		//Fill it with data
		freeFrames = sys_calculate_free_frames() ;
		intArr = (int*) ptr_allocations[8];
		lastIndexOfInt1 = (1*Mega + 512*kilo)/sizeof(int) - 1;

		i = 0;
		for (i=0; i <= lastIndexOfInt1 ; i++)
		{
			intArr[i] = i ;
		}
		if ((freeFrames - sys_calculate_free_frames()) != 384 + 1) panic("Wrong placement when accessing the allocated space");

		//Reallocate it to 2.5 MB [should be moved to next hole]
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();

		ptr_allocations[8] = realloc(ptr_allocations[8], 1*Mega + 512*kilo + 1*Mega - kilo);

		//[1] test return address & re-allocated space
		if ((uint32) ptr_allocations[8] !=(USER_HEAP_START + 8*Mega)) panic("Wrong start address for the re-allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 256) panic("Wrong re-allocation");
		if ((sys_calculate_free_frames() - freeFrames) != 384 + 1 ) panic("Wrong re-allocation: either extra pages are allocated in memory or pages not removed correctly when moving the reallocated block");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 256) panic("Extra or less pages are allocated in PageFile");

		//[2] test memory access
		freeFrames = sys_calculate_free_frames() ;
		lastIndexOfInt2 = (2*Mega + 512*kilo)/sizeof(int) - 1;
		intArr = (int*) ptr_allocations[8];

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
		if ((freeFrames - sys_calculate_free_frames()) != 384) panic("Wrong placement when accessing the allocated space");

		//[3] test freeing it after expansion
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		free(ptr_allocations[8]);
		//if ((sys_calculate_free_frames() - freeFrames) != 640) panic("Wrong free of the re-allocated space");
		if ((sys_calculate_free_frames()-freeFrames) != 256+384+1 ) panic("Wrong free after reallocation");
		if ((usedDiskPages- sys_pf_calculate_allocated_pages()) != 256+384) panic("Extra or less pages are removed from PageFile");

		cprintf("\nCASE2: (Re-allocate that's not fit in the same location) is succeeded...\n") ;

		/*CASE3: Re-allocate that's not fit in the same location*/

		//Fill it with data
		freeFrames = sys_calculate_free_frames() ;
		intArr = (int*) ptr_allocations[0];
		lastIndexOfInt1 = (1*Mega)/sizeof(int) - 1;

		i = 0;
		for (i=0; i <= lastIndexOfInt1 ; i++)
		{
			intArr[i] = i ;
		}
		if ((freeFrames - sys_calculate_free_frames()) != 256 + 1) panic("Wrong placement when accessing the allocated space");

		//Reallocate it to 4 MB [should be moved to last hole]
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[0] = realloc(ptr_allocations[0], 1*Mega + 3*Mega - kilo);

		//[1] test return address & re-allocated space
		if ((uint32) ptr_allocations[0] !=(USER_HEAP_START + 14*Mega)) panic("Wrong start address for the re-allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 768 + 1) panic("Wrong re-allocation");
		if ((sys_calculate_free_frames() - freeFrames) != 256 + 1 - 1) panic("Wrong re-allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 768) panic("Extra or less pages are allocated in PageFile");

		//[2] test memory access
		freeFrames = sys_calculate_free_frames() ;
		lastIndexOfInt2 = (4*Mega)/sizeof(int) - 1;
		intArr = (int*) ptr_allocations[0];
		for (i=lastIndexOfInt1+1; i < lastIndexOfInt2 ; i++)
		{
			intArr[i] = i ;
		}
		if ((freeFrames - sys_calculate_free_frames()) != 768 + 2) panic("Wrong placement when accessing the allocated space");

		freeFrames = sys_calculate_free_frames() ;
		for (i=0; i < lastIndexOfInt2 ; i++)
		{
			cnt++;
			if (intArr[i] != i) panic("Wrong re-allocation: stored values are wrongly changed!");
		}
		if ((freeFrames - sys_calculate_free_frames()) != 256) panic("Wrong placement when accessing the allocated space");

		//[3] test freeing it after expansion
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		free(ptr_allocations[0]);
		//if ((sys_calculate_free_frames() - freeFrames) != 1024+1) panic("Wrong free of the re-allocated space");
		if ((sys_calculate_free_frames() - freeFrames) != 1024+2) panic("Wrong re-allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 1024) panic("Extra or less pages are allocated in PageFile");

		("\nCASE3: (Re-allocate that's not fit in the same location) is succeeded...\n") ;
	}

	cprintf("Congratulations!! test realloc [1] is completed successfully.\n");

	return;
}
