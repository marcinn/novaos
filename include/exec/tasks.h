#ifndef EXEC_TASKS_H
#define EXEC_TASKS_H

#include <exec/types.h>
#include <exec/nodes.h>
#include <exec/lists.h>

struct Task {
	struct Node		tc_Node;
	UBYTE			tc_Flags;
	UBYTE			tc_State;
	BYTE			tc_IDNestCnt;
	BYTE			tc_TDNestCnt;
	ULONG			tc_SigAlloc;
	ULONG			tc_SigWait;
	ULONG			tc_SigRecvd;
	ULONG			tc_SigExcept;
	UWORD			tc_TrapAlloc;
	UWORD			tc_TrapAble;
	APTR			tc_ExceptData;
	APTR			tc_ExceptCode;
	APTR			tc_TrapData;
	APTR			tc_TrapCode;
	APTR			tc_SPReg;
	APTR			tc_SPLower;
	APTR			tc_SPUpper;
	void			(*tc_Switch)();
	void			(*tc_Launch)();
	struct List		tc_MemEntry;
	APTR			tc_UserData;
};

#define	TS_RUN			0
#define	TS_READY		1
#define	TS_WAIT			2
#define TS_ADDED		3
#define	TS_INVALID		4
#define	TS_EXCEPT		5
#define TS_REMOVED		6

#define	TS_RUNNING		TS_RUN
#define	TS_WAITING		TS_WAIT
#define	RUNNING			TS_RUN
#define	WAITING			TS_WAIT


#define TF_LAUNCH		(1<<0)
#define	TF_RUN			(1<<1)
#define	TF_END			(1<<2)
#define TF_EXCEPT		(1<<3)
#define TF_SWITCH		(1<<4)

#endif
