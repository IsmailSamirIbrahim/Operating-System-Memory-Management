#include <inc/lib.h>

void _main(void)
{	
	int envID = sys_getenvid();
	rsttst();
	volatile struct Env* myEnv;
	myEnv = &(envs[envID]);

	int Mega = 1024*1024;
	int kilo = 1024;
	void* ptr_allocations[20] = {0};
	int freeFrames ;
	//[1] Allocate all
	{
		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		ptr_allocations[0] = malloc(1*Mega-kilo);
		tst((uint32) ptr_allocations[0], USER_HEAP_START,USER_HEAP_START + PAGE_SIZE, 'b', 0);
		tst((freeFrames - sys_calculate_free_frames()) , 256+1 ,0, 'e', 0);

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		ptr_allocations[1] = malloc(1*Mega-kilo);
		tst((uint32) ptr_allocations[1], USER_HEAP_START + 1*Mega,USER_HEAP_START + 1*Mega + PAGE_SIZE, 'b', 0);
		tst((freeFrames - sys_calculate_free_frames()) , 256 ,0, 'e', 0);

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		ptr_allocations[2] = malloc(1*Mega-kilo);
		tst((uint32) ptr_allocations[2], USER_HEAP_START + 2*Mega,USER_HEAP_START + 2*Mega + PAGE_SIZE, 'b', 0);
		tst((freeFrames - sys_calculate_free_frames()) , 256 ,0, 'e', 0);

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		ptr_allocations[3] = malloc(1*Mega-kilo);
		tst((uint32) ptr_allocations[3], USER_HEAP_START + 3*Mega,USER_HEAP_START + 3*Mega + PAGE_SIZE, 'b', 0);
		tst((freeFrames - sys_calculate_free_frames()) , 256 ,0, 'e', 0);

		//Allocate 2 MB
		freeFrames = sys_calculate_free_frames() ;
		ptr_allocations[4] = malloc(2*Mega-kilo);
		tst((uint32) ptr_allocations[4], USER_HEAP_START + 4*Mega,USER_HEAP_START + 4*Mega + PAGE_SIZE, 'b', 0);
		tst((freeFrames - sys_calculate_free_frames()) , 512+1 ,0, 'e', 0);

		//Allocate 2 MB
		freeFrames = sys_calculate_free_frames() ;
		ptr_allocations[5] = malloc(2*Mega-kilo);
		tst((uint32) ptr_allocations[5], USER_HEAP_START + 6*Mega,USER_HEAP_START + 6*Mega + PAGE_SIZE, 'b', 0);
		tst((freeFrames - sys_calculate_free_frames()) , 512 ,0, 'e', 0);

		//Allocate 3 MB
		freeFrames = sys_calculate_free_frames() ;
		ptr_allocations[6] = malloc(3*Mega-kilo);
		tst((uint32) ptr_allocations[6], USER_HEAP_START + 8*Mega,USER_HEAP_START + 8*Mega + PAGE_SIZE, 'b', 0);
		tst((freeFrames - sys_calculate_free_frames()) , 768+1 ,0, 'e', 0);

		//Allocate 3 MB
		freeFrames = sys_calculate_free_frames() ;
		ptr_allocations[7] = malloc(3*Mega-kilo);
		tst((uint32) ptr_allocations[7], USER_HEAP_START + 11*Mega,USER_HEAP_START + 11*Mega + PAGE_SIZE, 'b', 0);
		tst((freeFrames - sys_calculate_free_frames()) , 768+1 ,0, 'e', 0);
	}

	//[2] Free some to create holes
	{
		//1 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		free(ptr_allocations[1]);
		tst((sys_calculate_free_frames() - freeFrames) , 256,0, 'e', 0);

		//2 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		free(ptr_allocations[4]);
		tst((sys_calculate_free_frames() - freeFrames) , 512,0, 'e', 0);

		//3 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		free(ptr_allocations[6]);
		tst((sys_calculate_free_frames() - freeFrames) , 768,0, 'e', 0);
	}
	int cnt = 0;

	//[3] Allocate again [test first fit]
	{
		//Allocate 512 KB - should be placed in 1st hole
		freeFrames = sys_calculate_free_frames() ;
		ptr_allocations[8] = malloc(512*kilo - kilo);
		tst((uint32) ptr_allocations[8], USER_HEAP_START + 1*Mega,USER_HEAP_START + 1*Mega + PAGE_SIZE, 'b', 0);
		tst((freeFrames - sys_calculate_free_frames()) , 128 ,0, 'e', 0);

		//Expand it
		freeFrames = sys_calculate_free_frames() ;
		realloc(ptr_allocations[8], 512*kilo + 256*kilo - kilo);
		tst((freeFrames - sys_calculate_free_frames()) , 64 ,0, 'e', 0);

		int *intArr = (int*) ptr_allocations[8];
		int lastIndexOfInt = ((512+256)*kilo)/sizeof(int) - 1;

		int i = 0;
		for (i=0; i < lastIndexOfInt ; i++)
		{
			cnt++;
			intArr[i] = i ;
		}

		for (i=0; i < lastIndexOfInt ; i++)
		{
			tst(intArr[i], i,0, 'e', 0);
		}

		freeFrames = sys_calculate_free_frames() ;
		free(ptr_allocations[8]);
		tst((sys_calculate_free_frames() - freeFrames) , 192 ,0, 'e', 0);
	}


	chktst(23 + cnt);

	return;
}
