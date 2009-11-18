%macro ReadFileM 3
	push	es
	push	bx
	push	si
	mov	bx,%2
	mov	es,bx
	mov	bx,%3
	mov	si,%1
	call	ReadFile
	pop	si
	pop	bx
	pop	es
%endmacro

%macro ErrorCheckM 1
	push	si
	mov	si,%1
	call	RF_ErrorCheck
	pop	si
%endmacro

%macro PrintM	1-*
%if %0>0
	push	si
	%rep %0
		mov	si,%1
		call	print
		%rotate 1
	%endrep
	pop	si
%endif
%endmacro
