#include <exec/types.h>

#define inportb(port)			\
{								\
    _u8 ret;    
    __asm__ __volatile__ ("inb %w1,%b0\n\tpushl %%ecx\n\t"
	    "movl $10,%%ecx\n.e1:\tloop .e1\n\tpopl %%ecx\n\t"
	    : "=a" (ret)
	    : "d" (port));
    return ret;
}

void	outportb(_u16 port, _u8 some)
{
    __asm__ __volatile__ ("outb %b0,%w1\n\tpushl %%ecx\n\t"
	    "movl $10,%%ecx\n.e2:\tloop .e2\n\tpopl %%ecx\n\t"
	    :
	    : "a" (some),
	      "d" (port));
}

_u32	inportl(_u16 port)
{
    _u32 ret;    
    __asm__  ("inl %%dx,%%eax\n\tpushl %%ecx\n\t"
	    "movl $10,%%ecx\n.e3:\tloop .e3\n\tpopl %%ecx\n\t"
	    : "=a" (ret)
	    : "d" (port));
    return ret;
}

void	outportl(_u16 port, _u32 some)
{
    __asm__ ("outl %0,%w1\n\tpushl %%ecx\n\t"
	    "movl $10,%%ecx\n.e4:\tloop .e4\n\tpopl %%ecx\n\t"
	    :
	    : "a" (some),
	      "d" (port));
}

