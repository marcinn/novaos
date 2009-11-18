#include <exec/types.h>
#include <exec/memory.h>
#include <exec/alerts.h>
#include <exec/exec_private.h>
#include <nova/libdef.h>

LIBFUNC1(void, Alert, ULONG, num, struct ExecBase *,_ExecBase)
{
	static const char* alerts[] =
	{
		"Unknown error",
		"Out Of Memory"
	};

	exec_dbug_clrscr();
	
	{
		char buf[80];
		_printf("%s\n",repc(0xcd,80,buf));
		if(num&AT_DEADEND) _printf("%-20s","Deadend alert:");
			else _printf("%-20s","Recoverable alert:");
		num&=~AT_DEADEND;
		_printf("%10#0X (%47s)\n", num, alerts[num]);
		_printf("%s\n",repc(0xcd,80,buf));
		
		__asm__ __volatile__ ("cli\n\thlt\n\t");
	}
}
LIBFUNCEND

