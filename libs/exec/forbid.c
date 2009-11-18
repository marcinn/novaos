#include <exec/types.h>
/* #include <exec/tasks.h> */
#include <exec/execbase.h>
#include <nova/libdef.h>

/* #include <proto/exec.h> */

LIBFUNC0(void, Forbid, struct ExecBase *,SysBase)
{
/*
	struct Task *task=FindTask(NULL);
	task->tc_TDNestCnt++;
*/
	SysBase->TDNestCnt++;
}
LIBFUNCEND
