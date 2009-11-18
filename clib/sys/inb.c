#include <system.h>

_u8	inportb(_u16 port)
{
    _u8 ret;    
    __asm__ __volatile__ ("inb %w1,%b0\n\tpushl %%ecx\n\t"
	    "movl $10,%%ecx\n.e1:\tloop .e1\n\tpopl %%ecx\n\t"
	    : "=a" (ret)
	    : "d" (port));
    return ret;
}

