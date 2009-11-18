#include <exec/tasks.h>
#include <exec/execbase.h>
#include <exec/lists.h>
#include <nova/libdef.h>

LIBFUNC0(void, Dispatch, struct ExecBase *,SysBase)
{
    struct Task *this, *task;

    this = SysBase->ThisTask;

    /* Check the stack... */
    /* Get the task which is ready to run */

    if(( task = (struct Task *)RemHead(&SysBase->TaskReady)))
    {
		if(this->tc_Flags & TF_SWITCH)
		{
			/* real switch here */
			/* do this->tc_Swich() */
		}

		this->tc_TDNestCnt = SysBase->TDNestCnt;
		this->tc_IDNestCnt = SysBase->IDNestCnt;
		
	/*  Oh dear, the previous task has just vanished...
	    you should have called Switch() instead :-)
		
	    We don't change the state of the old task, otherwise it
	    may never get freed.(See RemTask() for details).
		
	    We especially don't add it to the ReadyList !
	*/
		task->tc_State = TS_RUN;
		
		SysBase->TDNestCnt = task->tc_TDNestCnt;
		SysBase->IDNestCnt = task->tc_IDNestCnt;
		
		SysBase->ThisTask = task;

		/* Check the stack of the task we are about to launch */

		if( task->tc_SPReg <= task->tc_SPLower
	    	|| task->tc_SPReg >= task->tc_SPUpper )
		{
	    	/* POW! */
/*	    	Alert(AT_DeadEnd|AN_StackProbe); */

		}
		
		if(task->tc_Flags & TF_LAUNCH)
		{
			/* do task->tc_Launch() */
		}
		
		/* Increase the dispatched counter */
//		SysBase->DispCount++;
    }
    else
    {
		_printf("Eh? No tasks left to Dispatch()\n");
//		SysBase->IdleCount++;

    }

    /* Aha, the task pointed to be SysBase->ThisTask is correct. */
}
LIBFUNCEND
