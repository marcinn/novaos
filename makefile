TOP = /root/nova/sources
LIBTAIL = $(TOP)/libs/libtail.o

export TOP
export LIBTAIL

.PHONY: core fmbr
core:
	make -C core core

fmbr:
	make -C boot/floppy mbr

boot:
	make fmbr

stdlib:
	make -C clib std.lib

exec:
	make -C libs/exec exec.library

installboot:
	dd count=1 seek=0 if=obj/floppy-mbr of=/dev/fd0
		
kernel.o:
	make -C kernel kernel.o
	make clean
clean:
	make -C kernel clean
	make -C kernel/asm clean

df0:
	mount /dev/fd0
	cd /mnt/floppy

.c.o:
	gcc -c -o $*.o $*.c -Iinclud/e -nostdinc -fno-builtin
