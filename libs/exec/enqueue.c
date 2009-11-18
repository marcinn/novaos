#include <exec/types.h>
#include <exec/lists.h>
#include <nova/libdef.h>

LIBFUNC2(void, Enqueue, struct List *, list,
						struct Node *, node,
						struct ExecBase *,SysBase)
{
    struct Node *next;

    ForeachNode (list, next) {
		if (node->ln_Pri > next->ln_Pri)
	    break;
    }

    node->ln_Next			= next->ln_Prev;
    next->ln_Prev->ln_Next	= node;
    next->ln_Prev			= node;
    node->ln_Next			= next;

}
LIBFUNCEND
