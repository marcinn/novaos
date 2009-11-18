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

