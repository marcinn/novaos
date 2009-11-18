#include <exec/types.h>
#include <exec/tasks.h>
#include <exec/execbase.h>
// #include <proto/exec.h>
#include <nova/libdef.h>
#include <system.h>

LIBFUNC0(void, Switch, struct ExecBase *,SysBase)
{
	struct Task *next,*this;
	
	while(SysBase->TDNestCnt>0);	/* wait for schedule permission */

	/* schedule */
	
	this = SysBase->ThisTask;
	
	if( (this->tc_State == TS_RUN) && !(this->tc_Flags&TF_EXCEPT))
	{
		this->tc_State = TS_READY;
		Reschedule(this);
	}

	Dispatch();

/*
	SysBase->ThisTask->tc_IDNestCnt = SysBase->IDNestCnt;
	SysBase->ThisTask->tc_TDNestCnt = SysBase->TDNestCnt;
	
	next = struct (Task *) RemHead(SysBase->TaskReady);
	
	SysBase->TDNestCnt = next->tc_TDNestCnt;
	SysBase->IDNestCnt = next->tc_IDNestCnt;
*/	

}
LIBFUNCEND
