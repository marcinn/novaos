#include <exec/types.h>
#include <exec/execbase.h>
#include <nova/libdef.h>

/* #include <proto/exec.h> */

LIBFUNC0(void, Permit, struct ExecBase *,SysBase)
{
/*
	struct Task *task = FindTask(NULL);
	task->tc_TDNestCnt--;
*/
	SysBase->TDNestCnt--;
}
LIBFUNCEND
