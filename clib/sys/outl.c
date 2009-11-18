#include <system.h>

void	outportl(_u16 port, _u32 some)
{
    __asm__ ("outl %0,%w1\n\tpushl %%ecx\n\t"
	    "movl $10,%%ecx\n.e4:\tloop .e4\n\tpopl %%ecx\n\t"
	    :
	    : "a" (some),
	      "d" (port));
}

