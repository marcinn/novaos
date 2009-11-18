; ==============================================================
; noVa OS Loader code		       [CODE START AT 0000:1000]
; --------------------------------------------------------------

BITS 16


	%define	KERNEL_CS	kernel_cs-gdt
	%define	KERNEL_DS	kernel_ds-gdt
	%define	LINEAR_CS	linear_cs-gdt
	%define LINEAR_DS	linear_ds-gdt


	%define	ERR_CANTREADBB	0
	%define	ERR_FATALERROR	1
	%define	ERR_BADKERNEL	2
	%define	ERR_NO386	3
	%define	ERR_READERROR	4
	%define	ERR_KERNELNOTFOUND	5
	
	ORG	0x2000

	%include 'const.asm'
	%include 'boot/boot.i'
	%include 'boot/ia32.i'

	jmp	start

	%include 'fat12.asm'

;============================================
; USTALENIE TRYBU GRAFICZNEGO
;============================================

SetGFX:
	push	ax
	push	bx
	push	cx
	
	xor	ah,ah
	mov	al,GFX_MODE
	int	0x10
	mov	ah,11h		; zaladowanie tablicy znakow
	mov	al,TABLICA_ZNAKOW
	mov	bl,MATRYCA
	int	0x10
	mov	ah,0x05		; ustalenie aktywnej strony (0)
	mov	al,0
	int	0x10
	mov	ah,0x01		; ustalenie wygladu kursora (smut dla trybu graficznego niestety)
	mov	cx,0x001F
	int	0x10

	pop	cx
	pop	bx
	pop	ax
	ret

; ==============================================================
;               SPRAWDZAMY CPU CZY NIE GORSZY NIZ 386
; ==============================================================
CheckCPU:
	push	si
	push	eax
	push	ebx
	
	mov	si,0
	call	print
	mov	si,m_cpu
	call	print

	pushf				; flags na stos (backup)
	pushf				; jeszcze raz
	pop bx				; stare flagi do BX
	mov ax,bx			; teraz rowniez w AX
	xor ah,70h			; proba zmiany bitu 14 (NT)...
	push ax				; ...lub b13:b12 (IOPL)
	popf				; sprawdzmy czy flags
	pushf				; uwzglednil zmiany
	pop ax				; nowe FLAGS w AX
	popf				; odzyskujemy FLAGS z backupu
	xor ah,bh			; jesli zmieniono b14 lub b13:b12 mamy 32Bit CPU
	and ah,70h
	jne	.ok			; jesli rozne wyskocz, cpu spoko.
	mov	ax,ERR_NO386
	call	Error
.ok	
	pop	ebx
	pop	eax
	pop	si
	ret

; ==============================================================
; BOOT BLOCK LOAD
; ==============================================================
	
BBlockLoad:
	push	si
	push	ax
	mov	si,m_bbinfo
	call	print
	call	FAT12_GetBBlockInfo
	jnc	.ok1
	mov	ax,ERR_CANTREADBB
	call	Error
.ok1	pop	ax
	pop	si
	ret

; ==============================================================
; FIND KERNEL FILE
; ==============================================================
; OUT:
;	ax - first cluster of file

FindKernelFile:
	push	bx
	push	si
	push	di
	push	es

	mov	si,m_kernelload
	call	print

	mov	bx,INT13_READBUFFER_SEG
	mov	es,bx
	movzx	eax,word [root_dir_sec]
	mov	cx,[root_sectors]
.next	push	cx
	push	es
	push	eax
	mov	bx,INT13_READBUFFER_OFFS
	mov	di,1
	call	FAT12_ReadSectors
	jnc	.ok1
	mov	ax,ERR_READERROR
	call	Error
.ok1:	mov	di,INT13_READBUFFER_OFFS
	mov	si,kernelfile
	call	FAT12_FindFile
	cmp	ax,0
	jne	.ok2
	pop	eax
	add	eax,1
	pop	es
	pop	cx
	loop	.next

	pop	es
	mov	ax,ERR_KERNELNOTFOUND
	call	Error

.ok2:	mov	bx,ax
	pop	eax
	pop	es
	pop	cx
	mov	ax,bx	
	pop	es
	pop	di
	pop	si
	pop	bx
	ret


;===============================================================
;                             WRITE
;===============================================================
; calling BIOS INT 10h to print char(s)
; IN:
;	bl - char attributes
;	si - ptr to string
;
; NOTE: string must be terminated by 0

write:
	push	ax
;	push	cx
.l	lodsb
        cmp	al,0
	jz	.ew
;	mov	cx,1
        mov	ah,0Eh
;	xor	bh,bh
        int	10h
	jmp	.l
.ew:
;	pop	cx
	pop	ax
	ret

; print
; si - ptr to string
print:
	push	bx
	push	si
	cmp	si,0
	je	.skip
	call	write
.skip	pop	si
	pop	bx
	ret

Hexf:
;	eax	- numer
	push	ebx
	push	esi
	push	ecx

	mov	ecx,8
.p	rol	eax,4
	mov	ebx,eax
	and	ebx,0xF
	push	eax
	mov	si,hexchars
	add	si,bx
	mov	al,[si]
	mov	ah,0x0E
	int	0x10
	pop	eax
	loop	.p

	pop	ecx
	pop	esi
	pop	ebx
	ret
		

; ==============================================================
;                           S T A R T
; ==============================================================

start:

	call	SetGFX
	call	ResetFDC
	call	CheckCPU
	call	BBlockLoad
	call	FindKernelFile		; ax - first cluster

	push	es
	mov	bx,KERNEL_SEGMENT
	mov	es,bx
	mov	bx,KERNEL_OFFSET
.lloop	cmp	ax,0x0FEF
	jae	.endfile	
	push	es
	call	FAT12_GetNextCluster
	movzx	eax,ax
	mov	[next_cluster],bx
	mov	di,1
	pop	es
	push	es
	xor	bx,bx
	call	FAT12_ReadSectors
	jnc	.ok2
	pop	es
	mov	ax,ERR_READERROR
	call	Error
.ok2	pop	es
	mov	bx,[bytes_per_sec]
	shr	bx,4
	mov	ax,es
	add	ax,bx
	mov	es,ax
	mov	ax,[next_cluster]
	jmp	.lloop
.endfile:
	pop	es
	mov	si,m_kernelloaded
	call	print
	
L_PMode:
	mov	si,m_pmode
	call	print

EnableA20:
	cli			; 1. Disable Interrupts
	call	A20WaitBuf	; 2. Wait for empty buffer
	mov	al,0xAD		;
	out	0x64,al		; 3. Disable Keyboard
	call	A20WaitBuf	; 4. Wait...
	mov	al,0xD0
	out	0x64,al
	call	A20WaitBuf
	in	al,0x60
	push	ax
	call	A20WaitBuf
	mov	al,0xD1
	out	0x64,al
	call	A20WaitBuf
	pop	ax
	or	al,2
	out	0x60,al
	call	A20WaitBuf
	mov	al,0xAE
	out	0x64,al
	call	A20WaitBuf

	mov	ax,0
	mov	es,ax
	mov	ds,ax
	
	mov	di,GDT_ADDR
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

A20WaitBuf:
	in	al,64h
	test	al,0x02
	jnz	A20WaitBuf
	ret


; ==============================================================
;                  getkey - Czekaj na klawisz
; ==============================================================

waitkey:
	push	eax
	mov ah, 0               ; wait for key
	int 016h
	pop	eax
	ret

; ==============================================================
;                            Reboot
; ==============================================================

reboot:
	mov	si,m_rebootmsg		; napisz ze nastapi reboot
	call	print
	call	waitkey		        ; po nacisnieciu klawisza
	jmp	0xFFFF:0000		; reboot =)

Error:
;	ax	- errno

	mov	si,err_table
	sal	ax,2
	add	si,ax
	mov	si,[si]
	call	print
	call	reboot

next_cluster	dw	0
	
; ==============================================================
;                         DANE i TEKSTY
; ==============================================================
kernelfile	db	'NOVAKRNL   ',0
hexchars	db	'0123456789ABCDEF',0
m_kernelloaded	db	'  Kernel loaded ok.',13,10,0
m_kernelload	db	'Loading Kernel File...',13,10,0
m_pmode		db	'Switching to Protected Mode',13,10,0
m_cpu		db	'Checking CPU...',13,10,0
m_bbinfo	db	'Reading bootblock info',13,10,0
m_ok		db	'Ok!',13,10,0
m_rebootmsg	db	'Hit any key to reboot',0

me_No386	db	'NOVA OS requires 386+ CPU.',13,10,0
me_BadKernel	db	'Error: Kernel file corrupted.',13,10,0
me_FatalError	db	'FATAL ERROR',13,10,0
me_CantReadBB	db	'Can`t read bootblock!',13,10
me_ReadError	db	'Read Error!',13,10
me_KrnlNotFnd	db	'Kernel file not found!',13,10,0

err_table	dd	me_CantReadBB
		dd	me_FatalError
		dd	me_BadKernel
		dd	me_No386
		dd	me_ReadError
	    	dd	me_KrnlNotFnd

;=====================================================================

BITS 32		
PMODESTART:
	mov	ax,LINEAR_DS
	mov	ds,ax
	mov	es,ax
	mov	ss,ax
	mov	fs,ax
	mov	gs,ax
	xor	eax,eax
	mov	ax,sp
	mov	esp,eax

; interrupts

	mov	edi,IDT_ADDR
	mov	ecx,256
.il:	mov	eax,intNull
	mov	[edi],ax
	shr	eax,16
	mov	[edi+6],ax
	mov	word [edi+2],LINEAR_CS
	mov	word [edi+4],1000111000000000b
	add	edi,8
	loop	.il

	lidt	[idt_reg]

;	jmp	$			; <-------------- I'M HERE!! KILL ME!!!
	
; jumping to elf kernel

	mov	ebx,(KERNEL_SEGMENT*16)+KERNEL_OFFSET	; physical addr
	mov	edi,ebx
	mov	edx,[edi+Elf32_Ehdr.e_phoff]
	add	edi,edx
	mov	edx,[edi+Elf32_Phdr.p_offset] ; offset
	mov	eax,[edi+Elf32_Phdr.p_vaddr]  ; virtual addr
	xchg	eax,ebx		; eax - phys, ebx - virt
	sub	eax,ebx		; vtop = phys - vir
	add	eax,edx		; vtop += offset
	mov	edi,VTOP_PTR
	mov	[edi],eax	; vtop pod VTOP_PTR dla kodu w C
	
	mov	edi,GDT_ADDR+KERNEL_CS+2
	call	SetBase
	mov	edi,GDT_ADDR+KERNEL_DS+2
	call	SetBase
	lgdt	[gdt_reg]

	mov	ax,KERNEL_DS
	mov	ds,ax
	mov	ss,ax
	mov	es,ax
	mov	fs,ax
	mov	ax,LINEAR_DS
	mov	gs,ax
	mov	esp,0xC00000-0x5000
	
	jmp	KERNEL_CS:0xC00000		; oby sie wiodlo...

SetBase:
; in -	eax: base
;	edi: addr of descriptor's base 0:15

	stosw
	ror	eax,16
	stosb
	add	edi,2
	ror	eax,8
	stosb

	ror	eax,8
	ret

intNull:
	pusha
	mov	al,0x20
	out	0x20,al
	popa
	iret

	jmp	$

;======================================================================
; DATA
;======================================================================

idt_reg:	dw	256*8
		dd	IDT_ADDR
gdt_size:	dd	(end_gdt-gdt_size)/8
gdt:
null_seg:	dd	0,0
kernel_cs:	dw	0xff,0
		db	0,10011010b,11001111b,0
kernel_ds:	dw	0xff,0
		db	0,10010010b,11001111b,0
linear_cs:	dw	0xff,0
		db	0,10011010b,11001111b,0
linear_ds:	dw	0xff,0
		db	0,10010010b,11001111b,0
end_gdt:
;=====================================================
gdt_reg:	dw	end_gdt-gdt-1
		dd	GDT_ADDR
;=====================================================
