
OBJS = _startup.o

NOVADIR = ../..
DESTDIR = $(NOVADIR)/obj/libs
INCDIR = $(NOVADIR)/include
GCC = gcc
CFLAGS = -c -nostdinc -nostdlib -I $(INCDIR) -O3 -s -fno-ident -fvolatile
LFLAGS = -dN -n -Ttext 0xC00000 -s -S -x -q --no-gc-sections

dos.library:
	make $(OBJS)
	ld -o $(DESTDIR)/dos.library $(OBJS) $(LFLAGS)

.c.o:
	$(GCC) $(CFLAGS) -o $*.o $*.c

clean:
	rm -f *.o

