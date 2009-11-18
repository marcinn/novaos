#ifndef EXEC_FUNCS_H
#define EXEC_FUNCS_H

#include <exec/libraries.h>
#include <exec/execbase.h>
#include <exec/lists.h>
#include <nova/libdef.h>

extern APTR AllocMem(ULONG, ULONG, struct ExecBase *);
extern void FreeMem (void *, ULONG, struct ExecBase *);
extern ULONG Avail( ULONG, struct ExecBase *);
extern void Alert( ULONG, struct ExecBase *);
extern void Enable( struct ExecBase *);
extern void Disable( struct ExecBase *);
extern void Forbid( struct ExecBase *);
extern void Permit( struct ExecBase *);
extern struct Task *	FindTask( STRPTR, struct ExecBase *);
extern struct Library	* OpenLibrary( STRPTR, ULONG, struct ExecBase *);
extern void CloseLibrary( struct Library *, struct ExecBase *);
extern APTR AddTask( struct Task *, APTR, APTR, struct ExecBase *);
extern void RemTask( struct Task *, struct ExecBase *);
extern ULONG SetTaskPri( struct Task *, ULONG, struct ExecBase *);
extern void Enqueue( struct List *, struct Node *, struct ExecBase *);
extern struct Node	* RemHead( struct List *, struct ExecBase *);
extern struct Node	* RemTail( struct List *, struct ExecBase *);
extern void MakeFunctions( FUNCVEC*, ULONG, struct Library *, struct ExecBase *);

#endif
