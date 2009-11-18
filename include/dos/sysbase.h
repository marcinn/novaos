#ifndef DOS_SYSBASE_H
#define DOS_SYSBASE_H

/***********************************************

    DOS/SYSBASE.H
    20.12.2002 (c) NovaTeam
    
    Definitions of SYSBASE struct
    Author: Marcin Nowak
    
 **********************************************/

#include <exec/types.h>

#define SYSBASE_ADDR	0x1000

struct SysBase
{
    ULONG	sb_KrnlVTOP;
    ULONG	sb_KrnlPTOV;
};


#endif /* DOS_SYSBASE_H */

