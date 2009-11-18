#ifndef EXEC_LIBRARIES_H
#define EXEC_LIBRARIES_H

#include <exec/types.h>
#include <exec/nodes.h>

struct Library {
    struct  Node lib_Node;
    UBYTE   lib_Flags;
    UBYTE   lib_pad;
    UWORD   lib_NegSize;	    /* number of bytes before library */
    UWORD   lib_PosSize;	    /* number of bytes after library */
    UWORD   lib_Version;	    /* major */
    UWORD   lib_Revision;	    /* minor */
    UWORD   lib_pad1;		    /* make sure it is longword aligned */
    APTR    lib_IdString;	    /* ASCII identification */
    ULONG   lib_Sum;		    /* the checksum */
    UWORD   lib_OpenCnt;	    /* How many people use us right now ? */
    UWORD   lib_pad2;		    /* make sure it is longword aligned */
};

#endif
