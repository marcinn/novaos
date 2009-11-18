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

printnl:
	push	bx
	push	si
	call	print
	mov	si,m_nl
	call	write
	pop	si
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

hexchars	db	'0123456789ABCDEF',0


