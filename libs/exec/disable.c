#include <exec/types.h>
#include <exec/execbase.h>
#include <nova/libdef.h>

/* #include <proto/exec.h> */

LIBFUNC0(void, Disable, struct ExecBase *,SysBase)
{
/*
	struct Task *task;
	task=FindTask(NULL);
	if(task->tc_IDNestCnt==0)
		__asm__ __volatile__ ("cli\n\t");
	task->tc_IDNestCnt++;
*/
	if(SysBase->IDNestCnt==0)
		__asm__ __volatile__ ("cli\n\t");
	SysBase->IDNestCnt++;
}
LIBFUNCEND
