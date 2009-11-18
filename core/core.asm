; ==============================================================
; noVa OS Loader code		       [CODE START AT 0000:1000]
; --------------------------------------------------------------

BITS 16


	%define	KERNEL_CS	kernel_cs-gdt
	%define	KERNEL_DS	kernel_ds-gdt
	%define	LINEAR_CS	linear_cs-gdt
	%define LINEAR_DS	linear_ds-gdt

	%define	DEBUG	0



	%include 'core/memorymap.i'
	
	ORG	ADDR_CORE_START
	
	%include 'boot/boot.i'
	%include 'boot/ia32.i'
	%include 'boot/errors.i'
	%include '_macros.asm'
	%include 'core/exec_resident.i'



	jmp	start

	%include 'fat12.asm'
	%include 'setgfx.asm'
	%include 'checkcpu.asm'
	%include 'dbug_showbuffer.asm'
	%include 'bblockload.asm'
	%include 'findfileinroot.asm'	
	%include 'findfileindir.asm'
	%include 'findfile.asm'		
	%include 'output.asm'
	%include 'errors.asm'		
	%include 'readfile.asm'
	%include 'gatea20.asm'
	%include 'misc.asm'
			
; ==============================================================
;                           S T A R T
; ==============================================================


start:

	cld

	mov	ax,0
	mov	ds,ax
	mov	es,ax
	
	call	SetGFX
	call	ResetFDC
	call	CheckCPU
	call	BBlockLoad

	ReadFileM execlibfile,ADDR_TEMPBUFFER>>4,0
	ErrorCheckM execlibfile
	mov	eax,[file_size]
	mov	[exec_size],eax


%if DEBUG
	PrintM	m_kernelloaded
	PrintM	m_pmode	
	call	waitkey
%endif

	call	EnableA20

	cli	

; CREATING GD_TABLE

	mov	ax,0
	mov	es,ax
	mov	ds,ax
	
	mov	di,ADDR_GDT
	mov	si,gdt
	mov	ecx,gdt_size
cpy:	lodsd
	stosd
	loop	cpy
	lgdt	[gdt_reg]		; nowy GDT

	mov	eax,cr0
	or	al,IA32_CR0_PE
	mov	cr0,eax			; hop do trybu chronionego
		
	jmp	dword LINEAR_CS:PMODESTART ; dzampujemy do procki 32bit
	
	
; ==============================================================
;                         DANE i TEKSTY
; ==============================================================
execlibfile	db	'LIBS/EXEC.LIB',0
doslibfile	db	'LIBS/DOS.LIB',0
m_kernelloaded	db	'Kernel loaded ok.',13,10,0
m_kernelload	db	'Loading Kernel File...',13,10,0
m_pmode		db	'Switching to Protected Mode',13,10,0
m_cpu		db	'Checking CPU...',13,10,0
m_bbinfo	db	'Reading bootblock info',13,10,0
m_ok		db	'Ok!',13,10,0
m_rebootmsg	db	'Hit any key to reboot',0
m_nl		db	13,10,0


exec_size	dd	0
dos_size	dd	0

tempname	dd	0
rootdirq	db	0
next_cluster	dw	0
tempbuf		resb	255

;=====================================================================

	%include 'pmode.asm'
	
