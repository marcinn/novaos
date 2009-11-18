OBJS = do_printf.o strcmp.o strlen.o repc.o atoi.o
NOVADIR = $(TOP)
DESTDIR = $(NOVADIR)/clib
INCDIR = $(NOVADIR)/include
GCC = gcc
CFLAGS = -c -nostdinc -nostdlib -I $(INCDIR) -O3 -s -fno-ident -fvolatile
LFLAGS = -dN -s -r

.PHONY:	std.lib

std.lib:
	make $(OBJS)
	ld -o $(DESTDIR)/std.lib $(OBJS) $(LFLAGS)
	make clean
	make -C sys stdport.lib

.c.o:
	$(GCC) $(CFLAGS) -o $*.o $*.c

clean:
	rm -f *.o

