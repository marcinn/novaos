Error:
;	ax	- errno

	mov	si,err_table
	sal	ax,2
	add	si,ax
	mov	si,[si]
	call	print
	call	reboot

Error2:
	push	si
	push	eax
	mov	si,err_table
	sal	ax,2
	add	si,ax
	mov	si,[si]
	call	print
	pop	eax
	pop	si
	ret

me_No386	db	'NOVA OS requires 386+ CPU.',13,10,0
me_BadKernel	db	'Error: Kernel file corrupted.',13,10,0
me_FatalError	db	'FATAL ERROR',13,10,0
me_CantReadBB	db	'Can`t read bootblock!',13,10
me_ReadError	db	'Read Error!',13,10
me_KrnlNotFnd	db	'Kernel file not found!',13,10,0
me_FileNotFnd	db	'File not found: ',0

err_table	dd	me_CantReadBB
		dd	me_FatalError
		dd	me_BadKernel
		dd	me_No386
		dd	me_ReadError
	    	dd	me_KrnlNotFnd
		dd	me_FileNotFnd

