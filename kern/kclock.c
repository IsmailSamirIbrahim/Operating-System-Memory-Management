/* See COPYRIGHT for copyright information. */

/* The Run Time Clock and other NVRAM access functions that go with it. */
/* The run time clock is hard-wired to IRQ8. */

#include <inc/x86.h>
#include <inc/stdio.h>
#include <inc/isareg.h>
#include <inc/timerreg.h>

#include <kern/kclock.h>
#include <kern/picirq.h>

unsigned
mc146818_read(unsigned reg)
{
	outb(IO_RTC, reg);
	return inb(IO_RTC+1);
}

void
mc146818_write(unsigned reg, unsigned datum)
{
	outb(IO_RTC, reg);
	outb(IO_RTC+1, datum);
}


void
kclock_start(void)
{
	uint16 cnt0 = kclock_read_cnt0() ;

	/* initialize 8253 clock to interrupt N times/sec */
	outb(TIMER_MODE, TIMER_SEL0 | TIMER_RATEGEN | TIMER_16BIT);
	outb(TIMER_CNTR0, TIMER_DIV((1000/CLOCK_INTERVAL)) % 256);
	outb(TIMER_CNTR0, TIMER_DIV((1000/CLOCK_INTERVAL)) / 256);
//	cprintf("Timer Started: Counter0 Value = %x\n", cnt0 );

	//cprintf("	Setup timer interrupts via 8259A\n");
	irq_setmask_8259A(irq_mask_8259A & ~(1<<0));
	//cprintf("	unmasked timer interrupt\n");
}

void
kclock_resume(void)
{
	uint16 cnt0 = kclock_read_cnt0() ;
	//cprintf("Timer RESUMED: Counter0 Value = %x\n", cnt0 );

	outb(TIMER_MODE, TIMER_SEL0 | TIMER_RATEGEN | TIMER_16BIT);
	outb(TIMER_CNTR0, (uint8)(cnt0 & 0x00FF));
	outb(TIMER_CNTR0, (uint8)((cnt0>>8) & 0x00FF));
	cnt0 = kclock_read_cnt0() ;
	//cprintf("Timer RESUMED: Counter0 Value = %x\n", cnt0 );

	//cprintf("	Setup timer interrupts via 8259A\n");
	irq_setmask_8259A(irq_mask_8259A & ~(1<<0));
	//cprintf("	unmasked timer interrupt\n");
}

void
kclock_stop(void)
{
	//Read Status Register
	//outb(TIMER_MODE, 0xe0);
	//uint8 status = inb(TIMER_CNTR0) ;

	//outb(TIMER_MODE, TIMER_SEL0 | TIMER_16BIT);
	//outb(TIMER_CNTR0, 0x00) ;
	//outb(TIMER_CNTR0, 0x00) ;

	outb(TIMER_MODE, TIMER_SEL0 | TIMER_RATEGEN | TIMER_16BIT);

	//uint16 cnt0 = kclock_read_cnt0() ;


//	uint16 cnt0_before = kclock_read_cnt0() ;
//
//	int h, c = 0 ;
//		for (h = 0 ; h < 30000 ; h++)
//		{
//			c++ ;
//		}
//	uint16 cnt0_after = kclock_read_cnt0() ;
//	cprintf("Timer STOPPED: Counter0 Before Lag = %x, After lag = %x\n", cnt0_before, cnt0_after );


	irq_setmask_8259A(0xFFFF);
//	cprintf("Timer STOPPED: Counter0 Value = %x\n", cnt0 );
	//cprintf("Timer STOPPED: Status Value = %x\n", status);


}


uint16
kclock_read_cnt0(void)
{
	uint8 cnt0_lo =  inb(TIMER_CNTR0);
	uint8 cnt0_hi =  inb(TIMER_CNTR0);
	uint16 cnt0 = (cnt0_hi << 8) | cnt0_lo ;
	return cnt0 ;
}


uint16
kclock_read_cnt0_latch(void)
{
	uint8 old_mode = inb(TIMER_MODE) ;
	outb(TIMER_MODE, TIMER_SEL0 | TIMER_LATCH);

	uint8 cnt0_lo =  inb(TIMER_CNTR0);
	uint8 cnt0_hi =  inb(TIMER_CNTR0);
	uint16 cnt0 = (cnt0_hi << 8) | cnt0_lo ;
	outb(TIMER_MODE, old_mode);

	return cnt0 ;
}

// __inline struct uint64
// get_virtual_time()
// {

// /*
 // * 	uint16 cx;
	// uint16 dx;
		// __asm __volatile("int %3\n"
		// : "=c" (cx), "=d" (dx)
		// : "a" (0),
		  // "i" (26)
		  // //: "ax", "cx", "dx"
		// );
		// uint32 result = (cx<<16) | dx;
// */
	// //	uint32 oldVal = rcr4();
	// //	lcr4(0);


	// struct uint64 result;

	// __asm __volatile("rdtsc\n"
	// : "=a" (result.low), "=d" (result.hi)
	// );

	// /*
	// uint32 low;
	// uint32 hi;
	// uint32 cx,eaxp,ebxp,ecxp,edxp ;
	// //; read APERF
	// cpuid(6, &eaxp, &ebxp, &ecxp, &edxp);

// //	__asm __volatile("movl 6, %eax\ncpuid\n"//bt %%ecx, 0\nmov %%ecx,%0"
// //		//	:	"=c" (cx)
// //			//: "=a" (low), "=d" (hi)
// //		);

// //	__asm __volatile("rdmsr"
// //					: "=a" (low), "=d" (hi)
// //			);


	// //char* ptr=(char*)&ecxp;
	// //ptr[3]=0;
	// //cprintf("as str = %s\n", ptr);
	// cprintf("ax = %x, bx = %x, cx = %x, dx = %x\n", eaxp,ebxp,ecxp,edxp);
	// */

	// return result;
// }

