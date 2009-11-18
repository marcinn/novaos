#ifndef EXEC_NODES_H
#define EXEC_NODES_H

/***********************************************

    NODES.H
    20.12.2002 (c) NovaTeam
    
    Author: Marcin Nowak
    
 **********************************************/

#include <exec/types.h>


struct Node
{
    struct Node
		*ln_Next,
		*ln_Prev;
    char	*ln_Name;
    UBYTE	ln_Type;
    BYTE	ln_Pri;
};

struct MinNode
{
    struct MinNode	*mln_Next,
			*mln_Prev;
};

#endif /* EXEC_NODES_H */
