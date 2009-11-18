
; #
; # MEMORY MAP
; #
;
; #############################################################################
; #  Linear addr range    #  Size    # Size(dec) #  Desription                #
; #############################################################################
; | 0x0      - 0x3        | 0x04     |         4 | NULL                       |
; | 0x4      - 0x7        | 0x04     |         4 | ExecBase Ptr               |
; | 0x8      - 0x3F       | 0x38     |        56 | Global Desc.Table (GDT)    |
; | 0x40     - 0x1FF      | 0x1C0    |       448 | NovaOS Settings Area (SA)  |
; | 0x200    - 0x9FF      | 0x800    |      2048 | Interrupt Desc.Table (IDT) |
; | 0xA00    - 0xFFF      | 0x600    |      1536 | Core stack                 |
; | 0x1000   - 0x2FFF     | 0x2000   |      8192 | Reserved for Core/ExecBase |
; | 0x3000   - 0x9FFFF    | 0x8E000  |    643072 | Free mem                   |
; | 0xA0000  - 0xC7FFF    | 0x60000  |           | VGA mem                    |
; | 0xC8000  - 0xEFFFF    |          |           | ... nothing ... Do not use |
; | 0xF0000  - 0xFFFFF    |          |           | ROM motherboart BIOS - DNU |
; | 0x100000 - ...        |          |           | Extended free mem          |
; `---------------------------------------------------------------------------'


%define	ADDR_IDT		0x200
%define	ADDR_GDT		0x8
%define	ADDR_EXECBASEPTR	0x4
%define	ADDR_STACK_START	0xA00
%define	ADDR_STACK_END		0xFFF
%define	ADDR_SETTINGS		0x40
%define	ADDR_CORE_START		0x1000
%define	ADDR_CORE_END		0x2FFF
%define	ADDR_FREEMEM1_START	0x3000
%define	ADDR_FREEMEM1_END	0x7FFFF
%define	ADDR_VGA_MEM		0xA0000
%define	ADDR_EXTMEM		0x100000

; # RELATIVE ADDR's

%define	CORE_SEGMENT		0
%define	STACK_SEGMENT		0
%define	STACK_POINTER		ADDR_STACK_END

%define	INT13_READBUFFER_SEG	0x0
%define	INT13_READBUFFER_OFFS	ADDR_FREEMEM1_START
%define	INT13_FATBUFFER		(ADDR_FREEMEM1_START+0x2000)	; readbuffer size = 16 sectors (enough, nie?)

; ------- temp --------
%define	KERNEL_SEGMENT		0x1000
%define	KERNEL_OFFSET		0
; ---------------------

%define	DOSLIB			0x90000	; linear...
%define	EXECLIB			0x80000	; as above...
%define	EXECLIB_SEGMENT		EXECBASE>>4
%define	DOSLIB_SEGMENT		DOSBASE>>4

%define ADDR_TEMPBUFFER	0x10000

%define VIRTUAL_ADDR	0xC0000000

; ########################
; # GLOBAL SETTINGS AREA #
; ########################

%define	SA_BASE			ADDR_SETTINGS
%define	VTOP_PTR		(SA_BASE + 0x0)
%define	BOOTDRIVE_PTR		(SA_BASE + 0x4)

