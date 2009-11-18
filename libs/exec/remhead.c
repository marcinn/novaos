#include <exec/lists.h>
#include <nova/libdef.h>

LIBFUNC1(struct Node *, RemHead, struct List *, list, struct ExecBase *,SysBase)
{
	struct Node *node;

    node = list->lh_Head->ln_Next;
    if (node) {
		node->ln_Prev = (struct Node *)list;
		node = list->lh_Head;
		list->lh_Head = node->ln_Next;
    }

    return node;
}
LIBFUNCEND
