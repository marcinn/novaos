#include <exec/lists.h>
#include <nova/libdef.h>

LIBFUNC1(struct Node *, RemTail, struct List *, list, struct ExecBase *,SysBase)
{
    struct Node * node;

    if ( (node = GetTail (list)) ) {
		node->ln_Prev->ln_Next = node->ln_Next;
		node->ln_Next->ln_Prev = node->ln_Prev;
    }

    return node;
}
LIBFUNCEND
