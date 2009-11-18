	.file	"exec_int.c"
	.version	"01.01"
gcc2_compiled.:
	.comm	IntServerPtr,4,4
	.comm	IDT_REG,6,1
.text
	.align 4
.globl exec_init_idt
	.type	 exec_init_idt,@function
exec_init_idt:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	244(%eax), %eax
	movl	%eax, IntServerPtr
	call	exec_setintfuncs
	movw	$2048, IDT_REG
	movl	$512, IDT_REG+2
#APP
	lidt (IDT_REG)
	
#NO_APP
	leave
	ret
.Lfe1:
	.size	 exec_init_idt,.Lfe1-exec_init_idt
	.ident	"GCC: (GNU) 2.96 20000731 (Red Hat Linux 7.1 2.96-81)"
