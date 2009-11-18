#include <exec/types.h>
#include <exec/memory.h>
#include <exec/exec_private.h>
#include <exec/execbase.h>
#include <nova/libdef.h>

extern void Forbid();
extern void Permit();

LIBFUNC2(APTR, AllocMem, ULONG, size, ULONG, flags, struct ExecBase *,SysBase)
{
	struct	MemHeader *mh;
	struct	MemBlock *mb;
	ULONG	realsize=size+sizeof(struct MemHeader);

	Forbid();	
	mb=(struct MemBlock *) GetHead(&SysBase->FreeMemList);
	
	if(!mb) return (NULL);

	while(mb->mb_Size<realsize) {
		mb=(struct MemBlock *) GetNext(mb);
		if(!mb) return (NULL);
	}
	
	realsize=realsize+(realsize%MEM_BLOCKSIZE);
	mb->mb_Size -= realsize;
	mh=(struct MemHeader *) (mb+mb->mb_Size);
	mh->mh_Node.mln_Prev = NULL;
	mh->mh_Node.mln_Next = NULL;
	mh->mh_Magic = MEMF_MAGIC;
	mh->mh_Size = realsize;
	Permit();
	return ((APTR) mh);

}
LIBFUNCEND

