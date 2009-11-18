; ==============================================================
;                  getkey - Czekaj na klawisz
; ==============================================================

waitkey:
	push	eax
	mov ah, 0               ; wait for key
	int 016h
	pop	eax
	ret

; ==============================================================
;                            Reboot
; ==============================================================

reboot:
	PrintM	m_rebootmsg
	call	waitkey		        ; po nacisnieciu klawisza
	jmp	0xFFFF:0000		; reboot =)

