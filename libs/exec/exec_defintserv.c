#include <exec/types.h>
#include <exec/execbase.h>
#include <exec/keyboard.h>
#include <exec/lists.h>
#include <exec/interrupts.h>

void exec_defaultintserver(volatile REGS regs)
{
	struct Interrupt *i;
//	void (* func)();
	
	i=(struct Interrupt *) GetHead(&SysBase->Interrupts[regs.intn]);
	
	while(i) {
//		func=i->i_FuncPtr;
//		func();
		i->i_FuncPtr();
		i=(struct Interrupt *) GetNext(i);
	}

}
