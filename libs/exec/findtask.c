#include <exec/types.h>
#include <exec/execbase.h>
#include <nova/libdef.h>
#include <string.h>

LIBFUNC1(struct Task *, FindTask, STRPTR, name, struct ExecBase *,SysBase)
{
	if(name==NULL)
		return(SysBase->ThisTask);
	
	{
		struct	Task *task;
		
		task=(struct Task *) GetHead(&SysBase->TaskReady);
		while(task) {
			if(strcmp(task->tc_Node.ln_Name, name)==0)
				return(task);
			task=(struct Task *) GetNext(task);
		}
		task=(struct Task *) GetHead(&SysBase->TaskWait);
		while(task) {
			if(strcmp(task->tc_Node.ln_Name, name)==0)
				return(task);
			task=(struct Task *) GetNext(task);
		}
	}

	return(NULL);
}
LIBFUNCEND
