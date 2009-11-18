GFX_MODE	equ	0x03
TABLICA_ZNAKOW	equ	0x02
MATRYCA		equ	0x01	; <-LICZBA WIERSZY dla trybow graficznych!!!

%define READ_ATTEMPTS	0x5

; ELF32

%define	EI_NIDENT	16

    STRUC Elf32_Ehdr
	.e_ident	resb	EI_NIDENT
	.e_type		resb	2
	.e_machine	resb	2
	.e_version	resb	4
	.e_entry	resb	4
	.e_phoff	resb	4
	.e_shoff	resb	4
	.e_flags	resb	4
	.e_ehsize	resb	2
	.e_phentsize	resb	2
	.e_phnum	resb	2
	.e_shentsize	resb	2
	.e_shnum	resb	2
	.e_shstrndx	resb	2
    ENDSTRUC

    STRUC Elf32_Phdr
	.p_type		resb	4
	.p_offset	resb	4
	.p_vaddr	resb	4
	.p_paddr	resb	4
	.p_filesz	resb	4
	.p_memsz	resb	4
	.p_flags	resb	4
	.p_align	resb	4
    ENDSTRUC
        
%define	ELFMAG0		0x73
%define	ELFMAG13	'FLE'

	