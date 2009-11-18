#include <exec/types.h>
#include <exec/memory.h>
// #include <proto/exec.h>
#include "exec_funcs.h"
#include <nova/libdef.h>

void LibOpen();
void LibClose();
void LibExpunge();
void ExtFunc();

const FUNCVEC exec_functable[] = {
	(FUNCVEC) &Alert,
	(FUNCVEC) &AllocMem,
	(FUNCVEC) &Avail,
	(FUNCVEC) &Disable,
	(FUNCVEC) &Enable,
	(FUNCVEC) &Enqueue,
	(FUNCVEC) &FindTask,
	(FUNCVEC) &Forbid,
	(FUNCVEC) &Permit,
	(FUNCVEC) &RemHead,
	(FUNCVEC) &RemTail,
	(FUNCVEC) &MakeFunctions,
	(FUNCVEC) &ExtFunc,
	(FUNCVEC) &LibExpunge,
	(FUNCVEC) &LibClose,
	(FUNCVEC) &LibOpen,
	(FUNCVEC) 0
};


/*
FUNCTABLE_INIT(exec_functable)
FUNCTABLE_ENTRY	Alert
FUNCTABLE_ENTRY AllocMem
FUNCTABLE_ENTRY Avail
FUNCTABLE_ENTRY Disable
FUNCTABLE_ENTRY Enable
FUNCTABLE_ENTRY Enqueue
FUNCTABLE_ENTRY FindTask
FUNCTABLE_ENTRY Forbid
FUNCTABLE_ENTRY Permit
FUNCTABLE_ENTRY RemHead
FUNCTABLE_ENTRY RemTail
FUNCTABLE_END
*/

void init()
{
}

void LibOpen()
{
}

void LibClose()
{
}

void LibExpunge()
{
}

void ExtFunc()
{
}
