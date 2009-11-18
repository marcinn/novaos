	%define	RF_FILENOTFOUND	1
	%define	RF_READERROR	2

;=================================
ReadFile:
;=================================
; IN:
;	es:bx - dest. addr
;	ds:si - ptr to filename
; OUT:	ax - 0: ok
;	     1: filenotfound
;	     2: readerror

	push	eax
	push	ecx
	push	edi

	push	es
	
%if DEBUG
	push	di
	mov	di,si
	PrintM	m_readingfile,di
	pop	di
	call	waitkey
%endif

	call	FindFile
	cmp	ax,0
	je	.fnf

%if DEBUG	
	call	waitkey
%endif
	
	pop	es
.lloop	cmp	ax,0x0FEF
	jae	.endfile	
	push	es
	call	FAT12_GetNextCluster
	movzx	eax,ax
	mov	[next_cluster],bx
	mov	di,1
	pop	es
	push	es
	xor	bx,bx
	call	FAT12_ReadSectors
	pop	es
	jnc	.ok2
	mov	ax,RF_READERROR
	jmp	.end
.ok2:	mov	bx,[bytes_per_sec]
	shr	bx,4
	mov	ax,es
	add	ax,bx
	mov	es,ax
	mov	ax,[next_cluster]
	jmp	.lloop
.endfile:
	xor	ax,ax
	jmp	.end
.fnf:	mov	ax,RF_FILENOTFOUND
.end:	pop	edi
	pop	ecx
	pop	eax
	ret

;============================	
RF_ErrorCheck:
;============================
; IN: ax - error nr
;     si - ptr to filename

	cmp	ax,0
	je	.ok
	cmp	ax,RF_FILENOTFOUND
	jne	.cler
	mov	ax,ERR_FILENOTFOUND
	call	Error2
	call	printnl
	call	reboot
.cler:	cmp	ax,RF_READERROR
	mov	ax,ERR_READERROR
	call	Error
.ok:	ret

%if DEBUG
m_readingfile	db	'Loading file: ',0
%endif
