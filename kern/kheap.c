#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>

//---------------------------------------------------------------------------------------------------------------------------
struct Info{
	uint32 Address;
	uint32 Size;
	int Used;
	int Start;
};
uint32 Virtual_Addresses[1024*1024];
struct Info Allocated_Info[(KERNEL_HEAP_MAX-KERNEL_HEAP_START)/PAGE_SIZE];
struct Info Free_Holes[((KERNEL_HEAP_MAX-KERNEL_HEAP_START)/PAGE_SIZE)/2];
uint32 Free_Holes_Size=0;
uint32 Max_Size =(KERNEL_HEAP_MAX-KERNEL_HEAP_START)/PAGE_SIZE;
uint32 Last_Alloction =KERNEL_HEAP_START;
uint32 Index_Last=0;

//used at bestfit , firstfit and worstfit.
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
		Return_Address=(uint32*)(KERNEL_HEAP_START+(Start*PAGE_SIZE));
		Last_Alloction +=((i+1)-Index_Last)*PAGE_SIZE;
		Index_Last=(i+1);

		Allocated_Info[Start].Address=(uint32)Return_Address;
		Allocated_Info[Start].Size=ROUNDUP(Size,PAGE_SIZE);
		int k=Start;
		for(;k<Index_Last;++k)
			Allocated_Info[k].Used=1;
	}
	else{
		//looping back for search.
		int Sz=Index_Last;
		Index_Last=0;Last_Alloction=KERNEL_HEAP_START;
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
			Return_Address=(uint32*)(KERNEL_HEAP_START+(Start*PAGE_SIZE));
			Last_Alloction +=((X+1)-Index_Last)*PAGE_SIZE;
			Index_Last=(X+1);

			Allocated_Info[Start].Address=(uint32)Return_Address;
			Allocated_Info[Start].Size=ROUNDUP(Size,PAGE_SIZE);
			int k=Start;
			for(;k<Index_Last;++k){
				Allocated_Info[k].Used=1;
			}
		}
		else
			Return_Address=NULL;
	}

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
				Free_Holes[Free_Holes_Size].Address=(KERNEL_HEAP_START+(Start*PAGE_SIZE));
				Free_Holes[Free_Holes_Size].Size=(Counter*PAGE_SIZE);
				Free_Holes[Free_Holes_Size].Start=Start;
				Free_Holes_Size++;
			}
			Counter=0;
			Start=i+1;
		}
		if(i == (Max_Size - 1)){
			Free_Holes[Free_Holes_Size].Address=(KERNEL_HEAP_START+(Start*PAGE_SIZE));
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
				Free_Holes[Free_Holes_Size].Address=(KERNEL_HEAP_START+(Start*PAGE_SIZE));
				Free_Holes[Free_Holes_Size].Size=(Counter*PAGE_SIZE);
				Free_Holes[Free_Holes_Size].Start=Start;
				Free_Holes_Size++;
			}
			Counter=0;
			Start=i+1;
		}
		if(i == (Max_Size - 1)){
			Free_Holes[Free_Holes_Size].Address=(KERNEL_HEAP_START+(Start*PAGE_SIZE));
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
				Free_Holes[Free_Holes_Size].Address=(KERNEL_HEAP_START+(Start*PAGE_SIZE));
				Free_Holes[Free_Holes_Size].Size=(Counter*PAGE_SIZE);
				Free_Holes[Free_Holes_Size].Start=Start;
				Free_Holes_Size++;
			}
			Counter=0;
			Start=i+1;
		}
		if(i == (Max_Size - 1)){
			Free_Holes[Free_Holes_Size].Address=(KERNEL_HEAP_START+(Start*PAGE_SIZE));
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
		for(;j<Sz;++j)
			Allocated_Info[j].Used=1;
	}


	return (void*)Return_Address;

}


void* kmalloc(unsigned int size)
{
	uint32* Start_Alloc_Address;
	Start_Alloc_Address=Next_Fit(size);

	if(Start_Alloc_Address == NULL)
		return NULL;


	uint32 Start_Va=(uint32)Start_Alloc_Address;
	uint32 Num_Of_Page=(ROUNDUP(size,PAGE_SIZE)/PAGE_SIZE);
	struct Frame_Info *Ptr_Frame_Info ;
	int i=0;
	for(;i < Num_Of_Page;++i,Start_Va +=PAGE_SIZE){
		Ptr_Frame_Info =NULL;
		allocate_frame(&Ptr_Frame_Info);
		map_frame(ptr_page_directory,Ptr_Frame_Info,(void*)Start_Va,(PERM_PRESENT | PERM_WRITEABLE));
		uint32 Frame_Num=(to_physical_address(Ptr_Frame_Info)/PAGE_SIZE);
		Virtual_Addresses[Frame_Num]=Start_Va;
	}
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
void kfree(void* virtual_address)
{
	virtual_address = ROUNDDOWN(virtual_address, PAGE_SIZE);

	//get the size of the given allocation using its address
	int Index=find_allocation_Index((uint32)virtual_address);
	uint32 Size =Allocated_Info[Index].Size;
	uint32 Num_Of_Page=(ROUNDUP(Size,PAGE_SIZE)/PAGE_SIZE);

	int i=0;
	for (;i<Num_Of_Page;++i,virtual_address+=PAGE_SIZE){
		Virtual_Addresses[(kheap_physical_address((unsigned int)virtual_address)/PAGE_SIZE)]=0;
		unmap_frame(ptr_page_directory,virtual_address);
	}

	Allocated_Info[Index].Address=0;
	Allocated_Info[Index].Size=0;
	i=(Size/PAGE_SIZE);
	while(i--){
		Allocated_Info[Index].Used=0;
		++Index;
	}
}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//return the virtual address corresponding to given physical_address
	return Virtual_Addresses[(physical_address/PAGE_SIZE)];
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{

	uint32* Ptr_Page_Table = NULL;
	struct Frame_Info *Ptr_Fram_Info=NULL;

	Ptr_Fram_Info = get_frame_info(ptr_page_directory,(void*)virtual_address,&Ptr_Page_Table);
	uint32 Entry = Ptr_Page_Table[PTX(virtual_address)];
	uint32 Present = (Entry & PERM_PRESENT);

	//if page of virtual_address not present in main_memory.
	if(Ptr_Fram_Info == NULL || Present == 0 || Ptr_Page_Table == NULL)
		return 0;


	uint32 Frame_Num = (Entry >>12);
	uint32 Physical_Address =(Frame_Num * PAGE_SIZE);

	//return the physical_address corresponding to given virtual_address.
	return Physical_Address;
}
