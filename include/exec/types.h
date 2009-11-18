/***********************************************

    TYPES.H
    20.12.2002 (c) NovaTeam
    
    Definitons of NovaOS types
    Author: Marcin Nowak
    
 **********************************************/

#ifndef TYPES_H
#define	TYPES_H


    typedef	unsigned long	_u32;
    typedef	unsigned short	_u16;
    typedef	unsigned char	_u8;
    typedef	long		_32;
    typedef	short		_16;
    typedef	char		_8;

    typedef	_u32	ULONG;
    typedef	_u16	UWORD;
    typedef	_u8	UBYTE;
    typedef	_32	LONG;
    typedef	_16	WORD;
    typedef	_8	BYTE;
    
    typedef	void*	APTR;
	typedef	char*	STRPTR;

	typedef struct
	{
		 ULONG edi,esi,ebp,esp,ebx,edx,ecx,eax;
		 ULONG intn,errn;
		 ULONG eip,cs,eflags,user_esp,user_ss;
	} REGS;


	    
#ifndef NULL
#define	NULL	((void *) 0L)
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define	FALSE	0
#endif

    

#endif		// TYPES.H

