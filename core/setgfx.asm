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

