EnableA20:
	push	ax
	
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
	pop	ax
	sti
	ret
	
A20WaitBuf:
	in	al,64h
	test	al,0x02
	jnz	A20WaitBuf
	ret
