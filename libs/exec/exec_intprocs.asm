	%include '/core/memorymap.i'

extern IntServerPtr

exec_callserverproc:
	pusha
	mov		eax,IntServerPtr
	call	[eax]
	popa
	add		esp,8
	push	ax
	mov		al,0x20
	out		0x20,al
	pop		ax
	iret


%macro INTPROC 1
INTPRC%1:
	push	byte 0
	push	byte %1
	jmp		near exec_callserverproc
%endmacro

%assign i 0
%rep 256
INTPROC	i
%assign i i+1
%endrep

%macro FUNC 1
	mov	eax,INTPRC%1
	mov	bl,%1
	call kSetInt
%endmacro

kSetInt:
;	eax - addr
;	bl - intnr
	pusha
	mov	edi,ADDR_IDT
	and	ebx,0xFF
	shl	ebx,3
	add	edi,ebx
	mov	[ds:edi],ax
	shr	eax,16
	mov	[ds:edi+6],ax
	mov	ax,8
	mov	[ds:edi+2],ax
	mov	word [ds:edi+4],1000111000000000b
	popa
	ret

global exec_setintfuncs
exec_setintfuncs:
	push	ebp
	mov		ebp,esp
	pusha
	mov		ecx,256
	%assign i 0
	%rep 256
	FUNC i
	%assign i i+1
	%endrep
	popa
	leave
	ret
