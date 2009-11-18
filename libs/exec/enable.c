#include <exec/types.h>
#include <exec/execbase.h>
#include <nova/libdef.h>

/* #include <proto/exec.h> */

LIBFUNC0(void, Enable, struct ExecBase *,SysBase)
{
/*
	struct Task *task;
	task=FindTask(NULL);
	task->tc_IDNestCnt--;
	if(task->tc_IDNestCnt==0)
		__asm__ __volatile__ ("sti\n\t");
*/
	SysBase->IDNestCnt--;
	if(SysBase->IDNestCnt==0)
		__asm__ __volatile__ ("sti\n\t");
}
LIBFUNCEND
