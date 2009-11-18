;==================================================================
; GET BOOTBLOCK INFO
;==================================================================
;	OUT:	C=1 error
;		C=0 ok.

FAT12_GetBBlockInfo:

	push	eax
	push	ebx
	push	edx
	push	es
	push	di

	mov	eax,0x345
;	call	Hexf
	
	xor	eax,eax
	mov	bx,INT13_READBUFFER_SEG
	mov	es,bx
	mov	bx,INT13_READBUFFER_OFFS

	mov	ah,2
	mov	al,1
	mov	cx,1
	xor	dh,dh
	mov	dl,0		; <- change this 2 boot_device
	int	0x13
	jc	.end
	
	mov	bx,INT13_READBUFFER_OFFS
	mov	ax,[es:bx+0x1A]
	mov	[heads],ax
	
;	call	Hexf
	
	mov	ax,[es:bx+0x0B]
	mov	[bytes_per_sec],ax
	
;	call	Hexf
	
	mov	al,[es:bx+0x0D]
	mov	[sec_per_block],al
	mov	ax,[es:bx+0x0E]
	mov	[sec_reserved],ax
	mov	al,[es:bx+0x10]
	mov	[fat_copies],al
	mov	ax,[es:bx+0x11]
	mov	[root_size],ax
	mov	ax,[es:bx+0x16]
	mov	[fat_size],ax
	mov	ax,[es:bx+0x18]
	mov	[sec_per_track],ax

; ------------------------------------- obliczamy polozenie glownego katalogu
;                                              i zapisujemy do [root_dir_sec]   
	movzx	ax,byte [fat_copies]
	mul	word [fat_size]
	add	ax,[sec_reserved]
	mov	[root_dir_sec],ax
	
	movzx	eax,ax
;	call	Hexf
; ----------------------------------- wyznaczamy ilosc sektorow na [root_dir]
	mov	ax,[root_size]
	shl	ax,5
	div	word [bytes_per_sec]
	mov	[root_sectors],ax
; ----------------------------------------- obliczmy poczatek obszaru danych
	add	ax,[root_dir_sec]
	mov	[data_start],ax

.end	pop	di
	pop	es
	pop	edx
	pop	ebx
	pop	eax
	ret
	
	
;==================================================================
; RESET FDC
;==================================================================

ResetFDC:
	push	ax
	xor	ah,ah	
	int	0x13
	pop	ax
	ret

;==================================================================
; FIND FILE IN SECT
;==================================================================
;
; in:
;	es:di - sector data
;	ds:si - ptr to filename	(11 CHARS!)
; out:
;	ax - start cluster of file (0: if fault)

FAT12_FindFile:
	push	ebx
	push	ecx

%if DEBUG
	PrintM	m_findfn,si
	call	waitkey
%endif
	
	mov	cx,[bytes_per_sec]
	sar	cx,5
	mov	ax,CORE_SEGMENT
	mov	es,ax
.searchfile:
	push	cx
	mov	cx,11
	push	si
	push	di
	repz	cmpsb
	jz	.found
	pop	di
	pop	si
	add	di,0x20
	pop	cx
	loop	.searchfile		; szukaj dalej w sektorze
	xor	ax,ax
	pop	ecx
	pop	ebx
	ret
.found:
	pop	di
	pop	si

	mov	eax,[di+0x1C]
	mov	[file_size],eax
	mov	ax,[di+0x1A]

	pop	cx
	pop	ecx
	pop	ebx
	ret
	
;==================================================================
; READ SECTORS
;==================================================================
;
; in:	es:bx - adres bufora
;	eax - numer sektora logicznego
;	di - ilosc sektorow

FAT12_ReadSectors:
	push	dx
	push	cx

	xor	dx,dx
	div	word [sec_per_track]	; sector = (logic mod sec_per_track) + 1
	inc	dl
	mov	[sector],dl
	xor	dx,dx
	div	word [heads]		; track = (logic / sec_per_track) / heads
	mov	[track],ax
	mov	[head],dl		; head = (logic / sec_per_track) mod heads
	mov	cx,READ_ATTEMPTS
	
.tryagain:
	push	cx
	xor	ch,ch
	mov	cl,[sector]
	mov	ax,[track]
	xchg	ah,al
	ror	al,2
	or	cx,ax
	mov	ax,di
	mov	ah,2
	mov	dl,[boot_device]
	mov	dh,[head]
	push	bx
	int	0x13
	jnc	.ok			; ok ?
	pop	bx
	pop	cx
	loop	.tryagain		; wszystkie proby zle
	jmp	.end
.ok	pop	bx

;	push	si
;	push	di
;	push	es
;	
;	mov	ax,0xB800
;	mov	es,ax
;	mov	di,0
;	mov	si,INT13_READBUFFER_OFFS
;	mov	cx,[bytes_per_sec]
;.pisz	lodsb
;	mov	ah,7
;	stosw
;	loop	.pisz
;
;	pop	es
;	pop	di
;	pop	si
;	
;	mov	ah,0
;	int	0x16

	pop	cx	; ten z tryagain
.end	pop	cx
	pop	dx
	ret

;==================================================================
; FAT12_GetNextCluster
;==================================================================
;
; in:	es:si - fat table
;	ax - cluster
; out:	ax - first sector of cluster
;	bx - next cluster

FAT12_GetNextCluster:
	push	edx
	push	ds
	push	es
		
	mov	dx,CORE_SEGMENT
	mov	ds,dx
	
;	push	es

;	mov	dx,0
;	mov	es,dx
	
	mov	bx,ax
	shl	bx,1
	add	bx,ax		; bx=(n*3)
	shr	bx,1		; bx=bx/2
	add	bx,INT13_FATBUFFER
	mov	dx,[ds:bx]		; dx = next cluster
	test	ax,1
	jnz	.shift	
	and	dx,0x0FFF
	jmp	.ok
.shift:	shr	dx,4
.ok:	push	dx		; next cluster
	sub	ax,2
	movzx	dx,byte [sec_per_block]
	mul	dx
	add	ax,[data_start]
	pop	bx		; next cluster
;	xchg	ax,bx
;	call	Hexf
;	xchg	ax,bx
	pop	es
	pop	ds
	pop	edx
	ret
	
boot_device	db	0
root_dir_sec	dw 	0
root_sectors	dw	0
logical_sector	dd	0
head		db	0
track		dw	0
sector		db	0
data_start	dw	0
bytes_per_sec:	dw	0
sec_per_block:	db	0
sec_reserved:	dw	0
fat_copies:	db	0
root_size:	dw	0
sec_all:	dw	0
disk_id:	db	0
fat_size:	dw	0
sec_per_track:	dw	0
heads:		dw	0
file_size:	dd	0

%if DEBUG
m_found		db	'found!',13,10,0
m_findfn	db	'FAT12FindFile: ',0
%endif
