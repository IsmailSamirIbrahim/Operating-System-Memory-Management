#include <inc/lib.h>

struct Info{
	uint32 Address;
	uint32 Size;
	uint32 Acu_Size;
	int Used;
	int Start;
};
struct Info Allocated_Info[(USER_HEAP_MAX-USER_HEAP_START)/PAGE_SIZE];
struct Info Free_Holes[((USER_HEAP_MAX-USER_HEAP_START)/PAGE_SIZE)/2];
uint32 Free_Holes_Size=0;
uint32 Max_Size =(USER_HEAP_MAX-USER_HEAP_START)/PAGE_SIZE;
uint32 Last_Alloction =USER_HEAP_START;
uint32 Index_Last=0;

void Sort(char Type){

	if(Type == 'B'){
		int step,k;
		for(step=0;step<Free_Holes_Size-1;++step){
			for(k=0;k<Free_Holes_Size-step-1;++k){
				if(Free_Holes[k].Size>Free_Holes[k+1].Size){
					struct Info temp=Free_Holes[k];
					Free_Holes[k]=Free_Holes[k+1];
					Free_Holes[k+1]=temp;
				}
			}
		}
	}
	else if(Type == 'F'){
		int step,k;
		for(step=0;step<Free_Holes_Size-1;++step){
			for(k=0;k<Free_Holes_Size-step-1;++k){
				if(Free_Holes[k].Address>Free_Holes[k+1].Address){
					struct Info temp=Free_Holes[k];
					Free_Holes[k]=Free_Holes[k+1];
					Free_Holes[k+1]=temp;
				}
			}
		}
	}
	else if(Type == 'W'){
		int step,k;
		for(step=0;step<Free_Holes_Size-1;++step){
			for(k=0;k<Free_Holes_Size-step-1;++k){
				if(Free_Holes[k].Size<Free_Holes[k+1].Size){
					struct Info temp=Free_Holes[k];
					Free_Holes[k]=Free_Holes[k+1];
					Free_Holes[k+1]=temp;
				}
			}
		}

	}

}

void* Next_Fit(uint32 Size){
	uint32 Num_Of_Pages=(ROUNDUP(Size,PAGE_SIZE)/PAGE_SIZE);
	int Needed_Page=0,Flage=0;

	int Start=Index_Last;
	int i=Index_Last;
	for(;i<Max_Size;++i){
		if(Allocated_Info[i].Used == 0)
			++Needed_Page;
		else{
			Needed_Page=0;
			Start=i+1;
		}
		if(Needed_Page == Num_Of_Pages){
			Flage=1;
			break;
		}
	}
	uint32* Return_Address;
	if(Flage){
		Return_Address=(uint32*)(USER_HEAP_START+(Start*PAGE_SIZE));
		Last_Alloction +=((i+1)-Index_Last)*PAGE_SIZE;
		Index_Last=(i+1);

		Allocated_Info[Start].Address=(uint32)Return_Address;
		Allocated_Info[Start].Size=ROUNDUP(Size,PAGE_SIZE);
		Allocated_Info[Start].Acu_Size=Size;
		int k=Start;
		for(;k<Index_Last;++k)
			Allocated_Info[k].Used=1;
	}
	else{
		int Sz=Index_Last;
		Index_Last=0;Last_Alloction=USER_HEAP_START;
		Needed_Page=0;Flage=0;Start=Index_Last;
		int X=0;
		for(;X<Sz;++X){
			if(Allocated_Info[X].Used == 0)
				++Needed_Page;
			else{
				Needed_Page=0;
				Start=X+1;
			}
			if(Needed_Page == Num_Of_Pages){
				Flage=1;
				break;
			}
		}
		if(Flage){
			Return_Address=(uint32*)(USER_HEAP_START+(Start*PAGE_SIZE));
			Last_Alloction +=((X+1)-Index_Last)*PAGE_SIZE;
			Index_Last=(X+1);

			Allocated_Info[Start].Address=(uint32)Return_Address;
			Allocated_Info[Start].Size=ROUNDUP(Size,PAGE_SIZE);
			Allocated_Info[Start].Acu_Size=Size;
			int k=Start;
			for(;k<Index_Last;++k){
				Allocated_Info[k].Used=1;
			}
		}
		else
			Return_Address=NULL;
	}
	//	cprintf("Acu_Size = %d      Size = %d    Diff = %d\n",Allocated_Info[Start].Acu_Size,Allocated_Info[Start].Size,Allocated_Info[Start].Size-Allocated_Info[Start].Acu_Size);
	return (void*)Return_Address;
}
void* Best_Fit(uint32 Size){
	uint32 Num_Of_Pages=(ROUNDUP(Size,PAGE_SIZE)/PAGE_SIZE);
	int Counter=0;
	Free_Holes_Size=0;

	int i=0,Start=0;
	for(;i<Max_Size;++i){
		if(Allocated_Info[i].Used == 0)
			++Counter;
		else{
			if(Counter){
				Free_Holes[Free_Holes_Size].Address=(USER_HEAP_START+(Start*PAGE_SIZE));
				Free_Holes[Free_Holes_Size].Size=(Counter*PAGE_SIZE);
				Free_Holes[Free_Holes_Size].Start=Start;
				Free_Holes_Size++;
			}
			Counter=0;
			Start=i+1;
		}
		if(i == (Max_Size - 1)){
			Free_Holes[Free_Holes_Size].Address=(USER_HEAP_START+(Start*PAGE_SIZE));
			Free_Holes[Free_Holes_Size].Size=(Counter*PAGE_SIZE);
			Free_Holes[Free_Holes_Size].Start=Start;
			Free_Holes_Size++;
		}
	}
	uint32* Return_Address=NULL;
	Sort('B');

	for(i=0;i<Free_Holes_Size;++i){
		if(Free_Holes[i].Size >= Size){
			Return_Address=(uint32*)Free_Holes[i].Address;
			break;
		}
	}

	if(Return_Address != NULL){
		int j=Free_Holes[i].Start;
		int Sz=(j+Num_Of_Pages);
		Allocated_Info[j].Address=(uint32)Return_Address;
		Allocated_Info[j].Size=ROUNDUP(Size,PAGE_SIZE);
		Allocated_Info[j].Acu_Size=Size;
		Allocated_Info[j].Start=Free_Holes[i].Start;
		for(;j<Sz;++j)
			Allocated_Info[j].Used=1;
	}


	return (void*)Return_Address;
}
void* First_Fit(uint32 Size){
	uint32 Num_Of_Pages=(ROUNDUP(Size,PAGE_SIZE)/PAGE_SIZE);
	int Counter=0;
	Free_Holes_Size=0;

	int i=0,Start=0;
	for(;i<Max_Size;++i){
		if(Allocated_Info[i].Used == 0)
			++Counter;
		else{
			if(Counter){
				Free_Holes[Free_Holes_Size].Address=(USER_HEAP_START+(Start*PAGE_SIZE));
				Free_Holes[Free_Holes_Size].Size=(Counter*PAGE_SIZE);
				Free_Holes[Free_Holes_Size].Start=Start;
				Free_Holes_Size++;
			}
			Counter=0;
			Start=i+1;
		}
		if(i == (Max_Size - 1)){
			Free_Holes[Free_Holes_Size].Address=(USER_HEAP_START+(Start*PAGE_SIZE));
			Free_Holes[Free_Holes_Size].Size=(Counter*PAGE_SIZE);
			Free_Holes[Free_Holes_Size].Start=Start;
			Free_Holes_Size++;
		}
	}
	uint32* Return_Address=NULL;
	Sort('F');

	for(i=0;i<Free_Holes_Size;++i){
		if(Free_Holes[i].Size >= Size){
			Return_Address=(uint32*)Free_Holes[i].Address;
			break;
		}
	}

	if(Return_Address != NULL){
		int j=Free_Holes[i].Start;
		int Sz=(j+Num_Of_Pages);
		Allocated_Info[j].Address=(uint32)Return_Address;
		Allocated_Info[j].Size=ROUNDUP(Size,PAGE_SIZE);
		Allocated_Info[j].Acu_Size=Size;
		Allocated_Info[j].Start=Free_Holes[i].Start;
		for(;j<Sz;++j)
			Allocated_Info[j].Used=1;
	}


	return (void*)Return_Address;
}
void* Worst_Fit(uint32 Size){
	uint32 Num_Of_Pages=(ROUNDUP(Size,PAGE_SIZE)/PAGE_SIZE);
	int Counter=0;
	Free_Holes_Size=0;

	int i=0,Start=0;
	for(;i<Max_Size;++i){
		if(Allocated_Info[i].Used == 0)
			++Counter;
		else{
			if(Counter){
				Free_Holes[Free_Holes_Size].Address=(USER_HEAP_START+(Start*PAGE_SIZE));
				Free_Holes[Free_Holes_Size].Size=(Counter*PAGE_SIZE);
				Free_Holes[Free_Holes_Size].Start=Start;
				Free_Holes_Size++;
			}
			Counter=0;
			Start=i+1;
		}
		if(i == (Max_Size - 1)){
			Free_Holes[Free_Holes_Size].Address=(USER_HEAP_START+(Start*PAGE_SIZE));
			Free_Holes[Free_Holes_Size].Size=(Counter*PAGE_SIZE);
			Free_Holes[Free_Holes_Size].Start=Start;
			Free_Holes_Size++;
		}
	}
	uint32* Return_Address=NULL;
	Sort('W');

	for(i=0;i<Free_Holes_Size;++i){
		if(Free_Holes[i].Size >= Size){
			Return_Address=(uint32*)Free_Holes[i].Address;
			break;
		}
	}

	if(Return_Address != NULL){
		int j=Free_Holes[i].Start;
		int Sz=(j+Num_Of_Pages);
		Allocated_Info[j].Address=(uint32)Return_Address;
		Allocated_Info[j].Size=ROUNDUP(Size,PAGE_SIZE);
		Allocated_Info[j].Acu_Size=Size;
		Allocated_Info[j].Start=Free_Holes[i].Start;
		for(;j<Sz;++j)
			Allocated_Info[j].Used=1;
	}


	return (void*)Return_Address;

}

void* malloc(uint32 size){

	uint32* Start_Alloc_Address;

	if(sys_isUHeapPlacementStrategyNEXTFIT())
		Start_Alloc_Address=Next_Fit(size);
	else if(sys_isUHeapPlacementStrategyBESTFIT())
		Start_Alloc_Address=Best_Fit(size);
	else if(sys_isUHeapPlacementStrategyFIRSTFIT())
		Start_Alloc_Address=First_Fit(size);
	else if(sys_isUHeapPlacementStrategyWORSTFIT())
		Start_Alloc_Address=Worst_Fit(size);

	if(Start_Alloc_Address == NULL)
		return NULL;


	sys_allocateMem((uint32)Start_Alloc_Address,size);

	return (void*)Start_Alloc_Address;
}


uint32 find_allocation_Index(uint32 Virtual_Address){
	int i=0;
	for(;i<Max_Size;++i){
		if(Allocated_Info[i].Address==Virtual_Address && Allocated_Info[i].Used){
			return i;
		}
	}
	return -1;
}
void free(void* virtual_address)
{
	virtual_address = ROUNDDOWN(virtual_address, PAGE_SIZE);
	int Index=find_allocation_Index((uint32)virtual_address);
	uint32 Size =Allocated_Info[Index].Size;


	Allocated_Info[Index].Address=0;
	Allocated_Info[Index].Size=0;
	int i=(Size/PAGE_SIZE);
	while(i--){
		Allocated_Info[Index].Used=0;
		++Index;
	}
	sys_freeMem((uint32)virtual_address,Size);

}


//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// realloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_moveMem(uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		which switches to the kernel mode, calls moveMem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		in "memory_manager.c", then switch back to the user mode here
//	the moveMem function is empty, make sure to implement it.

void update(void* virtual_address){

	virtual_address = ROUNDDOWN(virtual_address, PAGE_SIZE);
	int Index=find_allocation_Index((uint32)virtual_address);
	uint32 Size =Allocated_Info[Index].Size;

	Allocated_Info[Index].Address=0;
	Allocated_Info[Index].Size=0;
	int i=(Size/PAGE_SIZE);
	while(i--){
		Allocated_Info[Index].Used=0;
		++Index;
	}

}

void *realloc(void *virtual_address, uint32 new_size)
{
	uint32 *Return_Address;

	if(virtual_address == NULL){
		Return_Address=malloc(new_size);
	}
	else if(new_size == 0){
		free(virtual_address);
		Return_Address=NULL;
	}
	else{
		uint32 Index=find_allocation_Index((uint32)virtual_address);

		//Check if Fragment Fit Order_Size.
		uint32 Order_Size=(new_size - Allocated_Info[Index].Acu_Size);
		uint32 Fragment = (Allocated_Info[Index].Size - Allocated_Info[Index].Acu_Size);
		if(Order_Size <= Fragment){
			Allocated_Info[Index].Size=ROUNDUP(new_size,PAGE_SIZE);
			Allocated_Info[Index].Acu_Size +=Order_Size;
			Return_Address=(uint32*)Allocated_Info[Index].Address;
		}
		//Check if Size After Old_Allocated_Address Fit Order_Size.
		else{
			uint32 Num_Of_Pages=(ROUNDUP(Order_Size,PAGE_SIZE)/PAGE_SIZE);
			int Counter=0;
			int Start_Search=((Allocated_Info[Index].Size/PAGE_SIZE) + Allocated_Info[Index].Start);
			int Limit_Search=(Start_Search + Num_Of_Pages);
			for(;Start_Search < Limit_Search;++Start_Search){
				if(Allocated_Info[Start_Search].Used == 0)
					Counter++;
			}
			//If It Fit.
			if(Counter == Num_Of_Pages){
				sys_allocateMem((Allocated_Info[Index].Address + Allocated_Info[Index].Size),Order_Size);
				Allocated_Info[Index].Size=ROUNDUP(new_size,PAGE_SIZE);
				Allocated_Info[Index].Acu_Size +=Order_Size;
				Return_Address=(uint32*)Allocated_Info[Index].Address;
			}
			//If It Not_Fit Search For New Segment To Allocate New_Size.
			else{
				uint32* Temp = malloc(new_size);
				if(Temp == NULL)
					Return_Address=(uint32*)virtual_address;
				else{
					Return_Address=Temp;

					//Copy & Free
					sys_moveMem((uint32)virtual_address,(uint32)Temp,Allocated_Info[Index].Size);

					//update Array Of Allocations (Allocated_Info).
					update((void*)virtual_address);
				}
			}

		}
	}
	return (void*)Return_Address;
}
