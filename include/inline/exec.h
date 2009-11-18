#ifndef INLINE_EXEC_H
#define INLINE_EXEC_H

#include <exec/types.h>
#include <nova/libdef.h>

#define	Alert(__AlertNum)	\
		CF1NR(96,ULONG,__AlertNum,ExecBase)
#define AllocMem(__Size,__MemFlags)	\
		CF2(APTR,90,ULONG,__Size,ULONG,__MemFlags,ExecBase)
#define Avail(__MemFlags)	\
		CF1(ULONG,84,ULONG,__MemFlags,ExecBase)
#define Disable() \
		CF0NR(78,ExecBase)
#define Enable() \
		CF0NR(72,ExecBase)
#define Enqueue(__List, __Node)	\
		CF2NR(66,struct List *, __List, struct Node *, __Node, ExecBase)
#define FindTask(__Name)	\
		CF1(struct Task *, 60, STRPTR, __Name, ExecBase)
#define Forbid()	\
		CF0NR(54, ExecBase)
#define Permit()	\
		CF0NR(48, ExecBase)
#define RemHead(__List)	\
		CF1(struct Node *, 42, struct List *, __List, ExecBase)
#define RemTail(__List)	\
		CF1(struct Node *, 36, struct List *, __List, ExecBase)

#endif
