#include <exec/memory.h>
#include <exec/types.h>
#include <exec/execbase.h>
#include <system.h>

APTR IntServerPtr;
struct IdtRegStruc {
	UWORD size __PACKED;
	ULONG idt_addr __PACKED;
} IDT_REG;


extern void exec_setintfuncs();



void exec_init_idt()
{

	IntServerPtr=SysBase->IntServer;
	exec_setintfuncs();
	IDT_REG.size=256*8;
	IDT_REG.idt_addr=EXEC_IDT_ADDR;
	__asm__ __volatile__ (
				"lidt (%0)\n\t"
				:
				: "m" (IDT_REG)
				);
}

	
			
