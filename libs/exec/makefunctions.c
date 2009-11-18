#include <exec/types.h>
#include <exec/execbase.h>
#include <exec/lists.h>
#include <exec/libraries.h>
#include <nova/libdef.h>

/*
	Create library functions vector table.
	Enough amount of memory must be allocated.
*/

void _printf(STRPTR, ...);

LIBFUNC3(void, MakeFunctions,	FUNCVEC *, funclist,
								ULONG, numfunc,
								struct Library *, libbase,
								struct ExecBase *,SysBase)
								
{
	ULONG i;
	struct LibVec *vector;
	FUNCVEC	func;
	ULONG vtsize=(numfunc*sizeof(struct LibVec));
	
//	_printf("MF: numfunc=%i, funclist=%i\n",numfunc, funclist);
	
	vector = (struct LibVec *) 	(((ULONG) (libbase))-(vtsize));
//	_printf("vtsize=%i, libbase=%#X, vector=%#X\n",vtsize,libbase,vector);
	for(i=0;i<numfunc;i++)
	{
		vector->jmp_cmd	=	LIBVEC_JMPCMD;
		vector->entry	= ((*funclist)-((ULONG) vector)-(sizeof(struct LibVec)));
//		_printf("funcaddr: %#X, jmpcmd: %#X   ", vector->entry, vector->jmp_cmd);
//		_printf("%#X, ",vector);
		vector++;
		funclist++;
	}
}
LIBFUNCEND
