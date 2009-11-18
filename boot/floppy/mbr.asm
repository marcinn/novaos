%define	C_NORMAL	7

	%include 'boot/boot.i'
	%include 'core/memorymap.i'
		
	ORG 	0x7C00
	BITS 16

;====================================================================
; FLOPPY MBR HEADER
;====================================================================

	jmp 	short	start		; <--------------- don't change!
	nop				; <--------------- don't change!
	
		db	'IBM',0,0,'4.0'	; nazwa i numer oem (8 bajtow)
bytes_per_sec:	dw	512		; bajty/sektor
sec_per_block:	db	1		; sektory/cluster
sec_reserved:	dw	1		; sektory zarezerwowane (start FAT)
fat_copies:	db	2		; liczba tablic FAT
root_size:	dw	0xE0		; liczba pozycji w gl. katalogu
sec_all:	dw	0x0B40		; ogolna liczba sektorow na dysku
disk_id:	db	0xF0		; ID typu dysku
fat_size:	dw	9		; liczba sektorow tablicy FAT
sec_per_track:	dw	0x12		; liczba sektorow na sciezce
heads:		dw	2		; liczba glowic dysku
sec_hidden:	dd	0		; ukryte sektory
sec_all_huge:	dd	0
drive_num	db	0
reserved	db	0
signature	db	0x29
volume_id	dd	0x5a541826
volume_label	db	'NOVA OS    '
filesystem_id	db	'FAT12   '

;=====================================================================
; MACROS
;=====================================================================

%macro	print 2
	mov	bl,%2
	mov	si,%1
	call	write
%endmacro

;=====================================================================
; S T A R T
;=====================================================================

start:	jmp	0:start2
start2:	mov	[boot_device],dl
	xor	ax,ax
	mov	ds,ax
	mov	es,ax
	cli
	
	mov	di,BOOTDRIVE_PTR
	mov	[di],dl
	mov	ax,STACK_SEGMENT
	mov	ss,ax
	mov	sp,STACK_POINTER
	sti
	cld

	print	m_welcome, C_NORMAL

; ----------------------------------------------------- reset floppy
	xor	ah,ah	
	int	0x13
; ------------------------------------- obliczamy polozenie glownego katalogu
;                                              i zapisujemy do [root_dir_sec]   
	movzx	ax,byte [fat_copies]
	mul	word [fat_size]
	add	ax,[sec_reserved]
	mov	[root_dir_sec],ax
; ----------------------------------- wyznaczamy ilosc sektorow na [root_dir]
	mov	ax,[root_size]
	shl	ax,5
	div	word [bytes_per_sec]
	mov	[root_sectors],ax
; ----------------------------------------- obliczmy poczatek obszaru danych
	add	ax,[root_dir_sec]
	mov	[data_start],ax
; ----------------------------------------------- czytamy tablice FAT do RAM
 	movzx	eax, word [sec_reserved]
	mov	bx,INT13_FATBUFFER
	mov	di,word [fat_size]
	call	read_sector
; ----------------------------------- czytamy caly [root] az do odnalezienia
;                                     pliku z loaderem lub nie...

	movzx	eax,word [root_dir_sec]
	mov	[logical_sector],eax
	mov	cx,[root_sectors]

readroot:
	mov	bx,INT13_READBUFFER_OFFS
	mov	eax,[logical_sector]
	mov	di,1
	call	read_sector
	jc	near error
	push	cx
	mov	bx,INT13_READBUFFER_OFFS
	mov	cx,[bytes_per_sec]
	shl	cx,5

.searchfile:
	push	cx
	mov	si,bx
	mov	di,loaderfile
	mov	cx,11
	repz	cmpsb
	jz	.found
	add	bx,0x20
	pop	cx
	loop	.searchfile			; szukaj dalej w sektorze

	pop	cx
	add	dword [logical_sector],1
	loop	readroot			; laduj nastepny sektor [roota]
	print	m_nima,C_NORMAL
	print	loaderfile,C_NORMAL
	jmp	$

.found:
	mov	ax,[bx+0x1A]		; found it!  ax=pozycja w tabeli fat
	mov	bx,ADDR_CORE_START

readfile:
; ax - pozycja w tabeli fat
	cmp	ax,0x0FEF
	jae	.endfile		; cluster ozn koniec pliku, blad lub cos innego
	push	bx
	mov	bx,ax
	shl	bx,1
	add	bx,ax		; bx=(n*3)
	shr	bx,1		; bx=bx/2
	add	bx,INT13_FATBUFFER
	mov	dx,[bx]		; bx = next cluster
	test	ax,1
	jnz	.shift	
	and	dx,0x0FFF
	jmp	.read

.shift:	shr	dx,4
.read:	mov	[next_cluster],dx
	sub	ax,2
	movzx	dx,byte [sec_per_block]
	mul	dx
	add	ax,[data_start]
	pop	bx
	push	bx
	mov	di,1
	call	read_sector
	jc	error
	pop	bx
	add	bx,word [bytes_per_sec]
	mov	ax,[next_cluster]
	jmp	short readfile

.endfile:	
	jmp	ADDR_CORE_START		; jmp to loader!

error:	print	m_error,C_NORMAL
end:	jmp	short $

;==================================================================
; READ SECTOR
;==================================================================
;
; in:	es:bx - adres bufora
;	eax - numer sektora logicznego
;
read_sector:
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
	int	0x13
	jnc	.ok			; ok ?
	pop	cx
	loop	.tryagain		; wszystkie proby zle
	jmp	.end
.ok	pop	cx	; ten z tryagain
.end	pop	cx
	pop	dx
	ret

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
.l	lodsb
        cmp	al,0
	jz	.ew
        mov	ah,0Eh
        int	10h
	jmp	.l
.ew:	pop	ax
	ret
	
;===================================================================
; DATA
;===================================================================

boot_device	db	0
root_dir_sec	dw 	0
root_sectors	dw	0
logical_sector	dd	0
head		db	0
track		dw	0
next_cluster	dw	0
sector		db	0
data_start	dw	0

;===================================================================
; SYSTEM FILES
;===================================================================

loaderfile	db	'NOVACORE   ',0

;===================================================================
; MESSAGES
;===================================================================

m_welcome	db	13,10,'[NOVA-OS]',13,10,0
m_nima		db	'File not found: ',0
m_error		db	'Error',0
;==================================================================
; SIGNATURE
;==================================================================

	times	510-($-$$) db 0
	dw	0xAA55

