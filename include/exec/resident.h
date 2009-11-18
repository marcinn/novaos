#ifndef EXEC_RESIDENT_H
#define	EXEC_RESIDENT_H

#include <exec/types.h>


struct Resident
{
	UWORD			rt_MatchWord;
	struct Resident	*rt_MatchTag;
	APTR			rt_EndSkip;
	UBYTE			rt_Flags;
	UBYTE			rt_Version;
	UBYTE			rt_Type;		/* po chuj? */
	BYTE			rt_Pri;
	STRPTR			rt_Name;
	STRPTR			rt_IdString;
	APTR			rt_Init;
};

#define	RTC_MATCHWORD	0x4AFC

#define	RTF_SINGLETASK	(1<<0)
#define	RTF_SYSTEMBOOT	(1<<1)

#define	RTT_TASK		(1<<0)
#define	RTT_LIBRARY		(1<<1)
#define	RTT_DEVICE		(1<<2)
#define	RTT_HANDLER		(1<<3)

#endif
