#ifndef EXEC_INTERRUPTS_H
#define EXEC_INTERRUPTS_H

struct Interrupt {
	struct Node i_Node;
	void		(* i_FuncPtr)();
//	APTR		i_FuncPtr;
};

#endif
