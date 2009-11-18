
%if DEBUG

_showbuffer:
	push	es
	push	ds
	push	di
	push	bx
	push	ax
		
	mov	cx,[bytes_per_sec]
	mov	si,INT13_READBUFFER_OFFS
	mov	bx,INT13_READBUFFER_SEG
	mov	ds,bx
	mov	bx,0xB800
	mov	es,bx
	mov	di,0
;	mov	bx,31
.l:	lodsb
	mov	ah,0x07
	stosw
;	dec	bx
;	jz	.nl
	loop	.l
;	jmp	.end
;.nl	add	di,(80-32)<<1
;	loop	.l
	
.end	pop	ax
	pop	bx
	pop	di
	pop	ds
	pop	es
	ret


%endif
