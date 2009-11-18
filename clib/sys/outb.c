#include <system.h>

void	outportb(_u16 port, _u8 some)
{
    __asm__ __volatile__ ("outb %b0,%w1\n\tpushl %%ecx\n\t"
	    "movl $10,%%ecx\n.e2:\tloop .e2\n\tpopl %%ecx\n\t"
	    :
	    : "a" (some),
	      "d" (port));
}

