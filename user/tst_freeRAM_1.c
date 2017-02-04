#include <inc/lib.h>

struct MyStruct
{
	char a;
	short b;
	int c;
};
void _main(void)
{	
	int envID = sys_getenvid();

	volatile struct Env* myEnv;
	myEnv = &(envs[envID]);

	int Mega = 1024*1024;
	int kilo = 1024;
	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;

	void* ptr_allocations[20] = {0};
	{
		//Allocate 2 MB
		ptr_allocations[0] = malloc(2*Mega-kilo);
		char *byteArr = (char *) ptr_allocations[0];
		int lastIndexOfByte = (2*Mega-kilo)/sizeof(char) - 1;
		byteArr[0] = minByte ;
		byteArr[lastIndexOfByte] = maxByte ;

		//Allocate another 2 MB
		ptr_allocations[1] = malloc(2*Mega-kilo);
		short *shortArr = (short *) ptr_allocations[1];
		int lastIndexOfShort = (2*Mega-kilo)/sizeof(short) - 1;
		shortArr[0] = minShort;
		shortArr[lastIndexOfShort] = maxShort;

		//Allocate all remaining RAM (Here: it requires to free some RAM)
		int freeFrames = sys_calculate_free_frames() ;
		ptr_allocations[2] = malloc(freeFrames*PAGE_SIZE);
		int *intArr = (int *) ptr_allocations[2];
		int lastIndexOfInt = (freeFrames*PAGE_SIZE)/sizeof(int) - 1;
		intArr[0] = minInt;
		intArr[lastIndexOfInt] = maxInt;

		//Allocate 7 KB after freeing some RAM
		ptr_allocations[3] = malloc(7*kilo);
		struct MyStruct *structArr = (struct MyStruct *) ptr_allocations[3];
		int lastIndexOfStruct = (7*kilo)/sizeof(struct MyStruct) - 1;
		structArr[0].a = minByte; structArr[0].b = minShort; structArr[0].c = minInt;
		structArr[lastIndexOfStruct].a = maxByte; structArr[lastIndexOfStruct].b = maxShort; structArr[lastIndexOfStruct].c = maxInt;

		//Check that the values are successfully stored
		if (byteArr[0] 	!= minByte 	|| byteArr[lastIndexOfByte] 	!= maxByte) panic("Wrong allocation: stored values are wrongly changed!");
		if (shortArr[0] != minShort || shortArr[lastIndexOfShort] 	!= maxShort) panic("Wrong allocation: stored values are wrongly changed!");
		if (intArr[0] 	!= minInt 	|| intArr[lastIndexOfInt] 		!= maxInt) panic("Wrong allocation: stored values are wrongly changed!");

		if (structArr[0].a != minByte 	|| structArr[lastIndexOfStruct].a != maxByte) 	panic("Wrong allocation: stored values are wrongly changed!");
		if (structArr[0].b != minShort 	|| structArr[lastIndexOfStruct].b != maxShort) 	panic("Wrong allocation: stored values are wrongly changed!");
		if (structArr[0].c != minInt 	|| structArr[lastIndexOfStruct].c != maxInt) 	panic("Wrong allocation: stored values are wrongly changed!");


	}

	cprintf("Congratulations!! test freeRAM (1) completed successfully.\n");

	return;
}
