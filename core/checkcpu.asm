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
.ok:
	pop	ebx
	pop	eax
	pop	si
	ret

