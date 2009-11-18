;====================================
FindFile:
;====================================
;   IN:
;	ds:si - filename with full path
;   OUT:
;	ax - start cluster of file


	push	ebx
	push	ecx
	push	di
	push	es

	
; 1. czyszczenie bufora

	mov	[tempname],si
	call	_clearbuf

	mov	byte [rootdirq],1

%if DEBUG
	PrintM	si
	PrintM	[tempname]
%endif

; 2. szukanie znaku '/'

.mloop:
.loop	lodsb
	cmp	al,0
	je	.searchlast
	cmp	al,'/'
	jne	.loop
	mov	ax,[tempname]
	xchg	si,ax		; si znow wskazuje na path
	mov	[next_part],ax
	sub	ax,si		; ax - liczba znakow do wyciecia
	

; kopiuj do tempbuf pierwszy czlon
	call	_clearbuf
	movzx	ecx,ax
	sub	ecx,1

	mov	ax,CORE_SEGMENT
	mov	es,ax
	mov	ds,ax
	mov	si,[tempname]
	mov	di,tempbuf
	rep	movsb

%if DEBUG	
	PrintM	d1
	call	waitkey
%endif

%if DEBUG
	PrintM	przedsearch,tempbuf
%endif

	mov	si,tempbuf
	call	_searchnow
	cmp	ax,0
	je	.end 		; ax juz niewazne, bo _searchnow zapamietuje w [start_cluster]	

%if DEBUG	
	PrintM	d2
	call	waitkey
%endif

	mov	si,[next_part]
	mov	[tempname],si
	mov	byte [rootdirq],0
%if DEBUG
	PrintM	d3
%endif
	jmp	.mloop
;------------------------------------------

.searchlast:
;	pop	si

; szukaj kropki
%if DEBUG
	PrintM	d4
%endif
	mov	si,[tempname]
.extlp	lodsb
	cmp	al,0
	je	.noext
	cmp	al,'.'
	jne	.extlp
	mov	ax,[tempname]	; si
	xchg	si,ax
	sub	ax,si
	sub	ax,1		; ilosc znakow przed kropka

	call	_clearbuf
	mov	di,tempbuf
	movzx	ecx,ax
	rep	movsb
	add	si,1		; ominac kropke
	mov	cx,8
	sub	cx,ax
	add	di,cx
.cpy2	lodsb
	cmp	al,0
	je	.cpxt
	stosb
	jmp	.cpy2
.cpxt	mov	di,tempbuf
	push	di
.noext	pop	si
	call	_searchnow
.end	pop	es
	pop	di
	pop	ecx
	pop	ebx
	ret
	
_searchnow:
	push	di
	push	si
	
	mov	ax,CORE_SEGMENT
	mov	es,ax
	mov	ds,ax

%if DEBUG
	PrintM	sn
	PrintM	si
	PrintM	tempbuf
%endif

	mov	di,tempbuf
	cmp	si,di
	je	.ok2
	call	_clearbuf

%if DEBUG
	PrintM	sn2
%endif

.l3:	lodsb
	cmp	al,0
	jz	.ok2
	stosb
	jmp	.l3
.ok2:
%if DEBUG
	PrintM	sn3
%endif
	cmp	byte [rootdirq],0
	jne	.searchroot
%if DEBUG
	PrintM	snd
%endif
	mov	si,tempbuf
	mov	ax,[start_cluster]
	call	FindFileInDir
	jmp	.end
.searchroot:
%if DEBUG
	PrintM	snr
%endif
	mov	si,tempbuf
%if DEBUG
	PrintM	tempbuf
%endif
	call	FindFileInRoot
.end:	mov	word [start_cluster],ax
	pop	si
	pop	di
	ret

;---------------------
; czysc bufor
	
_clearbuf:
; 1. wypelnienie bufora spacjami
	push	di
	push	ecx
	push	ax

	mov	ax,CORE_SEGMENT
	mov	es,ax
	mov	di,tempbuf
	mov	ecx,11
	mov	al,' '		; spacja
	rep	stosb

	pop	ax
	pop	ecx
	pop	di
	ret
	

start_cluster:	dw	0
letter_pos:	dw	0
next_part:	dw	0

%if DEBUG
d1		db	'1',13,10,0
d2		db	'2',13,10,0
d3		db	'3',13,10,0
d4		db	'4',13,10,0
d5		db	'5',13,10,0
sn		db	'snow',13,10,0
sn2		db	'snow2',13,10,0
sn3		db	'snow3',13,10,0
snr		db	'snroot',13,10,0
snd		db	'sndir',13,10,0
przedsearch	db	'przed search: ',0
%endif
