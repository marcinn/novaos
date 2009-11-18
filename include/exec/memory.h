#ifndef EXEC_MEMORY_H
#define EXEC_MEMORY_H

#include <exec/lists.h>

/*
 #
 # MEMORY MAP
 #

 #############################################################################
 #  Linear addr range    #  Size    # Size(dec) #  Desription                #
 #############################################################################
 | 0x0      - 0x3        | 0x04     |         4 | NULL                       |
 | 0x4      - 0x7        | 0x04     |         4 | ExecBase Ptr               |
 | 0x8      - 0x3F       | 0x38     |        56 | Global Desc.Table (GDT)    |
 | 0x40     - 0x1FF      | 0x1C0    |       448 | NovaOS Settings Area (SA)  |
 | 0x200    - 0x9FF      | 0x800    |      2048 | Interrupt Desc.Table (IDT) |
 | 0xA00    - 0xFFF      | 0x600    |      1536 | Core stack                 |
 | 0x1000   - 0x2FFF     | 0x2000   |      8192 | Reserved for Core/ExecBase |
 | 0x3000   - 0x7FFFF    | 0x7D000  |    512000 | Free mem                   |
 | 0x80000  - 0x8FFFF    | 0x10000  |     65536 | Reserved for exec.library  |
 | 0x90000  - 0x9FFFF    | 0x10000  |     65536 | Reserved for dos.library   |
 | 0xA0000  - 0xC7FFF    | 0x60000  |           | VGA mem                    |
 | 0xC8000  - 0xEFFFF    |          |           | ... nothing ... Do not use |
 | 0xF0000  - 0xFFFFF    |          |           | ROM motherboart BIOS - DNU |
 | 0x100000 - ...        |          |           | Extended free mem          |
 `---------------------------------------------------------------------------'
*/

#ifndef	NULL
#define	NULL	((void *) 0L);
#endif

#define	EXECBASE_PTR		0x4
#define	EXEC_GDT_ADDR		0x8
#define	EXEC_IDT_ADDR		0x200
#define	EXEC_SETTINGS		0x40
#define	EXECBASE_ADDR		0x1000
#define	EXEC_FREEMEM1_START	0x3000
#define	EXEC_FREEMEM1_END	0x9FFFF
#define	EXEC_EXTMEM_START	0x100000
#define	EXEC_NULLPTR		0x0
#define	EXECLIB				0x80000
#define	DOSLIB				0x90000

/******************* SETTINGS *******************/

#define	ES_VTOP			EXEC_SETTINGS
#define	ES_BOOTDRIVE	(EXEC_SETTINGS+0x4)
#define	ES_MEMORYLIST	(EXEC_SETTINGS+0x8)


#define MEMF_MAGIC		0x4F8A7F2A
#define	MEMF_MAGICVAL	MEMF_MAGIC

/************************* PRIVATE **********************/

struct	MemBlock
{
	struct	Node	mb_Node;
	ULONG	mb_Magic;
	ULONG	mb_Size;
};

/********************************************************/

struct MemHeader
{
	struct MinNode	mh_Node;
	ULONG 			mh_Magic;
	ULONG 			mh_Size;
};

#define MEMF_ANY		(0)
#define	MEMF_PUBLIC		(1)		/* future extension for memory protection */
#define	MEMF_CLEAR		(1<<1)
#define	MEMF_LARGEST	(1<<2)
#define	MEMF_REVERSE	(1<<3)
#define	MEMF_NO_EXPUNGE	(1<<4)
#define	MEMF_KERNEL		(1<<5)

/* Avail */

#define	MEMF_LARGEST	(1<<6)


#define	MEM_BLOCKSIZE	8L

extern volatile struct List *MemList;

#endif	/* EXEC_MEMORY_H */
