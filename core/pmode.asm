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

	mov	edi,ADDR_IDT
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

;	cld
	mov	esi,ADDR_TEMPBUFFER
	mov	eax,[esi+Elf32_Ehdr.e_phoff]
	add	esi,eax
	mov	eax,[esi+Elf32_Phdr.p_offset]
	mov	esi,ADDR_TEMPBUFFER
	add	esi,eax			; esi - pierwsza instrukcja sekcji .text
	mov	edi,EXECLIB		; edi - cel
	mov	ecx,[exec_size]
	sub	ecx,eax
	rep	movsb		; przeniesienie exec'a pod jego adres z pominieciem naglowka

;	szukanie RTC_MATCHWORD

	mov	esi,EXECLIB
	mov	ecx,[exec_size]
search:
	cmp	word [esi],RTC_MATCHWORD
	je	.rt1
	add	esi,1
	loop search
	
	mov	edi,0xB8000
	mov	word [edi],' !'
	mov	word [edi+2],'! '
	mov	word [edi+4],' !'
	jmp	$
.rt1:
	mov	eax,[esi+ExtResident.rt_MatchTag]
	cmp	eax,esi
	je	.rtok
	loop search

.rtok:
	mov	eax,[esi+ExtResident.rt_Strap]
	mov	[.entry],eax

;	mov	ebx,EXECLIB; physical addr
;	mov	eax,[ebx+Elf32_Ehdr.e_entry]
;	mov	[.entry],eax
;	mov	esi,[ebx+Elf32_Ehdr.e_phoff]
;	mov	eax,[esi+Elf32_Phdr.p_offset]
;	mov	esi,ebx
;	mov	edi,esi
;	add	esi,eax
;	mov	ecx,0x10000
;.lll:
;	mov	al,[esi]
;	mov	[edi],al
;	add	esi,1
;	add	edi,1
;	loop	.lll
	
;	mov [.entry],eax	
;	mov	edi,ebx
;	mov	edx,[edi+Elf32_Ehdr.e_phoff]
;	add	edi,edx
;	mov	edx,[edi+Elf32_Phdr.p_offset] ; offset
;	mov	eax,[edi+Elf32_Phdr.p_vaddr]  ; virtual addr
;	xchg	eax,ebx		; eax - phys, ebx - virt
;	sub	eax,ebx		; vtop = phys - vir
;	add	eax,edx		; vtop += offset
;	mov	edi,VTOP_PTR
;	mov	[edi],eax	; vtop pod VTOP_PTR dla kodu w C
;	
;	mov	edi,ADDR_GDT+KERNEL_CS+2
;	call	SetBase
;	mov	edi,ADDR_GDT+KERNEL_DS+2
;	call	SetBase
;	lgdt	[gdt_reg]

	mov	ax,KERNEL_DS
	mov	ds,ax
	mov	ss,ax
	mov	es,ax
	mov	ax,LINEAR_DS
	mov	fs,ax
	xor	ax,ax
	mov	gs,ax
	mov	esp,ADDR_STACK_END
;	mov	esp,VIRTUAL_ADDR-0x5000

	
	db	0xEA		; jmp...
.entry:	dd	0x0
	dw	KERNEL_CS
	
;	jmp	KERNEL_CS:VIRTUAL_ADDR		; oby sie wiodlo...

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

kSetInt:
;	eax - addr
;	bl - intnr
	pushf
	push	edi
	mov	edi,IDT_ADDR
	and	ebx,0xFF
	shl	ebx,3
	add	edi,ebx
	mov	[ds:edi],ax
	shr	eax,16
	mov	[ds:edi+6],ax
	mov	ax,8
	mov	[ds:edi+2],ax
	mov	word [ds:edi+4],1000111000000000b
	pop	edi
	popf
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
		dd	ADDR_IDT
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
		dd	ADDR_GDT
;=====================================================

