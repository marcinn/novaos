#ifndef EXEC_PRIVATE_H
#define EXEC_PRIVATE_H

#include <exec/types.h>
#include <exec/execbase.h>

#define KERNEL_CS	8
#define	KERNEL_DS	16
#define	LINEAR_CS	24
#define LINEAR_DS	32

struct regs_s
{
    ULONG user_esp, user_ss;
    ULONG fs,es,ds;
    ULONG edi,esi,ebp,esp,ebx,edx,ecx,eax;
    ULONG eip, cs, eflags;
};


extern ULONG _peekl(UWORD sel, ULONG addr);
extern UWORD _peekw(UWORD sel, ULONG addr);
extern UBYTE _peekb(UWORD sel, ULONG addr);
extern void _pokel(UWORD sel, ULONG addr, ULONG dana);
extern void _pokew(UWORD sel, ULONG addr, UWORD dana);
extern void _pokeb(UWORD sel, ULONG addr, UBYTE dana);
extern void _disable();
extern void _enable();
extern void _die();
extern void exec_dbug_clrscr();
extern void _printf(char *, ...);
extern void exec_mh_addblock(ULONG,ULONG,struct ExecBase *);
extern void exec_dbug_showlibs(struct ExecBase *);
extern UBYTE exec_GetCMOSByte(UBYTE);
extern UWORD exec_GetCMOSWord(UBYTE);

/* DEBUG */

extern volatile UWORD _cursx, _cursy, _tcols, _trows;

#define	EXEC_DEBUGVIDEOADDR		0xB8000
#define	EXEC_DEBUGCON_COLS		80
#define	EXEC_DEBUGCON_ROWS		25
#define EXEC_DEBUGCON_EMPTYCHAR	0x0720

#define	CRT_ADDR_REG	0x3D4
#define CRT_DATA_REG	0x3D5

#define CRT_CURSOR_START	0x0A
#define CRT_CURSOR_END		0x0B
#define CRT_CURSOR_LOC_HIGH	0x0E
#define	CRT_CURSOR_LOC_LOW	0x0F

#ifdef DEBUG
#define	D(var) var
#else
#define D(var)
#endif
#define MAKESTR(a,b) (a ## b)
#define bug(var)	_printf(var)

struct ExtResident
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
	APTR			rt_Strap;
};


#endif /* EXEC_PRIVATE_H */

