;==================
; CR0 REGISTER BITS
;==================

IA32_CR0_PG	EQU	1 << 31		; Paging (1: on)
IA32_CR0_CD	EQU	1 << 30		; Caching disabled
IA32_CR0_NW	EQU	1 << 29		; No write-through (writeback) disabled
IA32_CR0_AM	EQU	1 << 18		; Aligment check
IA32_CR0_WP	EQU	1 << 16		; Write-protect
IA32_CR0_NE	EQU	1 << 5		; External x87 FPU error reporting
IA32_CR0_TS	EQU	1 << 3		; No task switch
IA32_CR0_EM	EQU	1 << 2		; x87 FPU instructions not trapped
IA32_CR0_MP	EQU	1 << 1		; WAIT/FWAIT instructions not trapped
IA32_CR0_PE	EQU	1		; Protection Enabled

	