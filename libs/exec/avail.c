#include <exec/types.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <nova/libdef.h>

void _printf(STRPTR, ...);

LIBFUNC1(ULONG, Avail, ULONG, flags, struct ExecBase *, SysBase)
{
	struct MemBlock *mb;

//	_printf("Avail jest!\n");
//	_printf("flags: %#X, SysBase: %#X\n",flags, SysBase);
		
	switch (flags) {
		case MEMF_LARGEST:
		{
			ULONG largest;
			
			mb=(struct MemBlock *) GetHead(&SysBase->FreeMemList);
			if(!mb) return(0);
			largest=mb->mb_Size;

			while(mb=(struct MemBlock *) GetNext(mb))
				if(largest<mb->mb_Size) largest=mb->mb_Size;
//			_printf("largest end.\n");			
			return(largest);
		}
		break;
		
		case MEMF_PUBLIC:
		case MEMF_ANY:
		default:
		{
			ULONG size;
			
			mb=(struct MemBlock *) GetHead(&SysBase->FreeMemList);
			if(!mb) return(0);
			size=mb->mb_Size;

			while(mb=(struct MemBlock *) GetNext(mb))
				size+=mb->mb_Size;
			
//			_printf("memf_any/p end\n");
			return(size);
		}
		break;
	}
LIBFUNCEND
}

