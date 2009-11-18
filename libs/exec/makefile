.SUFFIXES: .asm

DEBUG = -DDEBUG
# LIBCALL_BY_REGS = -DNOVA_LIBCALL_PARAMS_BY_REGS
# LIBFUNC_BY_REGS = -DNOVA_LIBFUNC_PARAMS_BY_REGS

DEFINES = $(DEBUG) $(LIBCALL_BY_REGS) $(LIBFUNC_BY_REGS)

OBJS = exec_boot.o allocmem.o freemem.o avail.o addtask.o remtask.o \
    openlibrary.o closelibrary.o opendevice.o closedevice.o exec_private.o \
	alert.o findtask.o forbid.o permit.o enable.o disable.o enqueue.o \
	remhead.o remtail.o switch.o dispatch.o reschedule.o makefunctions.o \
	exec_init.o makelibrary.o exec_intprocs.o exec_int.o exec_defintserv.o \
	keyboard.o

NOVADIR = $(TOP)
DESTDIR = $(NOVADIR)/obj/libs
INCDIR = $(NOVADIR)/include
STDLIB = $(NOVADIR)/clib/std.lib $(NOVADIR)/clib/sys/stdport.lib
GCC = gcc
#CFLAGS = -nostdinc -nostdlib -I $(INCDIR) -O3 -s -fno-ident -fvolatile -fno-default-inline $(DEFINES)
CFLAGS = -nostdinc -nostdlib -I $(INCDIR) -O3 -s -fno-ident -fno-default-inline $(DEFINES)
LFLAGS = -dN -Ttext 0x80000 -s

exec.library:
	make $(OBJS)
	ld -o $(DESTDIR)/exec.lib $(OBJS) $(STDLIB) $(LIBTAIL) $(LFLAGS)
	make clean

.asm.o:
	nasm -i $(INCDIR) -f aout $*.asm -o $*.o

.c.o:
	$(GCC) -c $(CFLAGS) -o $*.o $*.c

clean:
	rm -f *.o

