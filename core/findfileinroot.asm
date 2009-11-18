; ==============================================================
; FIND FILE IN ROOT DIR
; ==============================================================
; IN:	
;	si - filename
; OUT:
;	ax - first cluster of file


FindFileInRoot:
	push	bx
	push	di
	push	es
	push	cx
	
	mov	[tempname],si

%if DEBUG
	PrintM	ffir,[tempname]
%endif
	
	mov	bx,INT13_READBUFFER_SEG
	mov	es,bx
	movzx	eax,word [root_dir_sec]
	mov	cx,[root_sectors]
.next:	push	cx
	push	es
	push	eax
%if DEBUG
	PrintM	sr1
%endif
	mov	bx,INT13_READBUFFER_OFFS
	mov	di,1
	call	FAT12_ReadSectors
	jnc	.ok1
	mov	ax,ERR_READERROR
	call	Error
.ok1:
%if DEBUG
	PrintM	sr2
%endif
	mov	di,INT13_READBUFFER_OFFS
	mov	si,[tempname]
	call	FAT12_FindFile
	cmp	ax,0
	jne	.ok2
	pop	eax
	add	eax,1
	pop	es
	pop	cx
	loop	.next
%if DEBUG
	PrintM	sr3
%endif
;	pop	es
	xor	ax,ax
	jmp	.end
.ok2:
%if DEBUG
	PrintM	sr4
%endif
	mov	bx,ax
	pop	eax
	pop	es
	pop	cx
	mov	ax,bx	
.end:
%if DEBUG
	PrintM	sr5
%endif
	pop	cx
	pop	es
	pop	di
	pop	bx
	ret

%if DEBUG
sr1	db	'sr1',13,10,0
sr2	db	'sr2',13,10,0
sr3	db	'sr3',13,10,0
sr4	db	'sr4',13,10,0
sr5	db	'sr5',13,10,0
sr6	db	'sr6',13,10,0
ffir	db	'FindFileInRoot: ',0
%endif
