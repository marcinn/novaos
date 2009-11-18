#ifndef PROTO_EXEC_H
#define PROTO_EXEC_H

#include <exec/types.h>
#include <exec/libraries.h>
#include <exec/tasks.h>
#include <nova/libdef.h>

/*
#define	LIBCALL(base, offset) \
		__asm__ __volatile__ __inline__ ("mov	%1,%%edx\n\t"
										 "call	offset(%%edx)\n\t"
ULONG AllocMem(ULONG size, ULONG flags)
*/

/* moved to "exec_funcs.h"
extern LIBFUNC2(APTR, AllocMem, ULONG, ULONG);
extern LIBFUNC2(void, FreeMem, void *, ULONG);
extern LIBFUNC1(ULONG, Avail, ULONG);
extern LIBFUNC1(void, Alert, ULONG);
extern LIBFUNC0(void, Enable);
extern LIBFUNC0(void, Disable);
extern LIBFUNC0(void, Forbid);
extern LIBFUNC0(void, Permit);
extern LIBFUNC1(struct Task *,	FindTask, STRPTR);
extern LIBFUNC2(struct Library	*, OpenLibrary, STRPTR, ULONG);
extern LIBFUNC1(void, CloseLibrary, struct Library *);
extern LIBFUNC3(APTR, AddTask, struct Task *, APTR, APTR);
extern LIBFUNC1(void, RemTask, struct Task *);
extern LIBFUNC2(ULONG, SetTaskPri, struct Task *, ULONG);
extern LIBFUNC2(void, Enqueue, struct List *, struct Node *);
extern LIBFUNC1(struct Node	*, RemHead, struct List *);
extern LIBFUNC1(struct Node	*, RemTail, struct List *);
extern LIBFUNC3(void, MakeFunctions, FUNCVEC *funclist, ULONG listsize, struct Library *libbase);
*/

#define	AllocMem(
#endif

