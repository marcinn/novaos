
OBJS = inb.o inw.o inl.o outb.o outw.o outl.o

NOVADIR = $(TOP)
DESTDIR = $(NOVADIR)/clib/sys
INCDIR = $(NOVADIR)/include

GCC = gcc
CFLAGS = -c -nostdinc -nostdlib -I $(INCDIR) -O3 -s -fno-ident -fvolatile
LFLAGS = -dN -Ttext 0xC0000000 -s -r

stdport.lib:
	make $(OBJS)
	ld -o $(DESTDIR)/stdport.lib $(OBJS) $(LFLAGS)
	make clean

.c.o:
	$(GCC) $(CFLAGS) -o $*.o $*.c

clean:
	rm -f *.o

