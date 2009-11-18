%ifndef	NOVAOS_CONSTANS
%define	NOVAOS_CONSTANS


;    %define	DEBUG		1

    %define ADR_START		7C00h

;    %define STARTUP_CS		0x0
;    %define STARTUP_DS		STARTUP_CS
;    %define STARTUP_OFFSET	0x9000
;    %define STARTUP_LINEAR	0x9000
;    %define STARTUP_SIZE	4

    %define KERNEL_SEGMENT	0x1000
    %define KERNEL_OFFSET	0x0

    %define LOADER_SEGMENT	0x0
    %define LOADER_OFFSET	0x2000

    %define NOVA_MAGICVAL 	'AvOn'
    %define REALMODE_SS		0x9000
    %define REALMODE_SP		0xFFFF
    %define GDT_ADDR		0x500
    %define IDT_ADDR		0x700
    %define VTOP_PTR		0x1000

    %define EXEC_SEG		0x200
    %define EXEC_OFF		0x0
    %define DOS_SEG		0x500
    %define DOS_OFF		0x0
    
%endif

