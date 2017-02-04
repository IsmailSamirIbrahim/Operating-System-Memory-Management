// Called from entry.S to get us going.
// entry.S already took care of defining envs, pages, vpd, and vpt.

#include <inc/lib.h>

extern void _main(int argc, char **argv);

volatile struct Env *env;
char *binaryname = "(PROGRAM NAME UNKNOWN)";

void
libmain(int argc, char **argv)
{
	// set env to point at our env structure in envs[].
	// env = envs;

	// save the name of the program so that panic() can use it
	if (argc > 0)
		binaryname = argv[0];

	// call user main routine
	_main(argc, argv);

	int envID = sys_getenvid();
	volatile struct Env* myEnv;
	myEnv = &(envs[envID]);

	sys_disable_interrupt();
		cprintf("**************************************\n");
		cprintf("Num of PAGE faults = %d\n", myEnv->pageFaultsCounter);
		cprintf("**************************************\n");
	sys_enable_interrupt();

	// exit gracefully
	exit();
}

