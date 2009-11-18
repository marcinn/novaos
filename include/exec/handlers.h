#ifndef EXEC_HANDLERS_H
#define EXEC_HANDLERS_H

#include <exec/types.h>
#include <exec/nodes.h>

struct	Handler {
	struct Node hnd_Node;
	APTR		hnd_FuncPtr;
};

#endif
