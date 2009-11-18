#include <exec/tasks.h>
#include <exec/execbase.h>
#include <exec/types.h>
#include <exec/lists.h>
#include <nova/libdef.h>

LIBFUNC1(void, Reschedule, struct Task *, task, struct ExecBase *,SysBase)
{
    switch(task->tc_State)
    {
	case TS_READY:
            Enqueue(&SysBase->TaskReady, (struct Node *)task);
            break;

	case TS_ADDED:
	    Enqueue(&SysBase->TaskReady, (struct Node *)task);
	    break;
	case TS_WAIT:
	    AddTail(&SysBase->TaskWait, (struct Node *)task);
	    break;

	case TS_REMOVED:
	    /* Ideally we should free the task here, but we can't
	       because that would corrupt the memory lists.
	    */
	    break;

	case TS_INVALID:
	case TS_EXCEPT:
	case TS_RUN:
	    /* We should never be called with this state. */
	    break;
    }
}
LIBFUNCEND
