;====================================
FindFileInDir:
;====================================
;   IN:
;	si - ptr to filename (8+3 with spaces!)
;	ax - first cluster of dir
;   OUT:
;	ax - first cluster of file

	push	si
	push	di
	push	bx
	push	cx
	
	mov	[tempname],si	; backup
%if DEBUG
	call	printnl
%endif
	
.loop	cmp	ax,0x0FEF
	jae	.endfile	
	call	FAT12_GetNextCluster
	movzx	eax,ax
	mov	[next_cluster],bx
	mov	di,1
	push	es
	mov	bx,INT13_READBUFFER_SEG
	mov	bx,es
	mov	bx,INT13_READBUFFER_OFFS
	call	FAT12_ReadSectors
	pop	es
	jnc	.ok2
	mov	ax,ERR_READERROR
	call	Error
.ok2:
;	call	_showbuffer
;	call	waitkey
	
	push	es
	mov	si,[tempname]
	mov	ax,INT13_READBUFFER_SEG
	mov	es,ax
	mov	di,INT13_READBUFFER_OFFS
	call	FAT12_FindFile
	pop	es
	cmp	ax,0
	jne	.end
	mov	ax,[next_cluster]
	jmp	.loop
	
.endfile:
	xor	ax,ax
.end	pop	cx
	pop	bx
	pop	di
	pop	si
	ret

