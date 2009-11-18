#ifndef EXEC_BASE_H
#define EXEC_BASE_H

/***********************************************

    EXECBASE.H
    31.12.2002 (c) NovaTeam
    
    Definitions of Lists
    Author: Marcin Nowak
    
 **********************************************/

#include <exec/libraries.h>
#include <exec/types.h>
#include <exec/lists.h>
#include <exec/interrupts.h>
#include <exec/tasks.h>

#define MAX_HANDLED_INTERRUPTS	256

struct ExecBase
{
    struct Library	LibNode;

    APTR	SysStkUpper;  /* Linear addr of stack top */
    APTR	SysStkLower;  /* Linear addr of stack bottom */
    ULONG	MemSize;	/* Size of installed memory (in bytes) */
    struct Task	*ThisTask;       /* Pointer to currently running task */
    ULONG       IdleCount;
    UWORD       Quantum;        /* # of ticks, a task may run */
    UWORD       Elapsed;        /* # of ticks, the current task has run */
    UWORD       SysFlags;
	BYTE	IDNestCnt;
	BYTE	TDNestCnt;
    ULONG	CPUFlags;
    APTR	CPUNamePtr;
    APTR	CPUVendorPtr;

    APTR	TaskTrapCode;
    APTR	TaskExceptCode;
    APTR	TaskExitCode;
    ULONG	TaskSigAlloc;
    UWORD	TaskTrapAlloc;

    struct List	FreeMemList;	/* list of freemem blocks */
    struct List	DeviceList;	/* opened devices */
    struct List	LibList;		/* opened libraries */
	struct List ResidentList;
	struct List Interrupts[MAX_HANDLED_INTERRUPTS];
    struct List	PortList;	/* ports */
    struct List	TaskReady;      /* Tasks that are ready to run */
    struct List	TaskWait;       /* Tasks that wait for some event */
    struct List	SemaphoreList;	/* jeszcze nie wiem jak tego uzywac */

	APTR	IntServer;
    APTR	MemHandler;     /* Pointer to memhandler (sbrk) */
};

extern struct ExecBase *SysBase;
extern struct ExecBase *ExecBase;

#endif /* EXEC_BASE_H */
