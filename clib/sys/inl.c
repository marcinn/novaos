#include <system.h>

_u32	inportl(_u16 port)
{
    _u32 ret;    
    __asm__  ("inl %%dx,%%eax\n\tpushl %%ecx\n\t"
	    "movl $10,%%ecx\n.e3:\tloop .e3\n\tpopl %%ecx\n\t"
	    : "=a" (ret)
	    : "d" (port));
    return ret;
}
