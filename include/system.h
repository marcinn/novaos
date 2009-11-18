#ifndef SYSTEM_H
#define SYSTEM_H

/***********************************************

    SYSTEM.H
    21.12.2002 (c) NovaTeam
    
    NovaOS System Definitions & functions header
    Author: Marcin Nowak
    
 **********************************************/

#include <exec/types.h>

#define KERNEL_CS	8
#define	KERNEL_DS	16
#define	LINEAR_CS	24
#define LINEAR_DS	32

extern _u8 inportb(_u16 port);
extern void outportb(_u16 port, _u8 some);
extern _u32 inportl(_u16 port);
extern void outportl(_u16 port, _u32 some);

#define	__TEXT			__attribute__ ((section (".text")))
#define __NORETURN		__attribute__ ((noreturn))
#define __PACKED __attribute__((packed))


#endif /* SYSTEM_H */

