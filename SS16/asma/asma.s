	.file	"asma.c"
	.text
	.globl	asma
	.type	asma, @function
asma:
.LFB11:
	.cfi_startproc
	movq	%rdx, %rcx              # copy parameter C from rdx to rcx (64 bit operation)
	movq	%rdi, %rax              # copy  parameter A from  rdi to rax
	imul	%rsi              # multiply parameter A  in register SI by AX and store it in AX 
	idiv	%rcx		     #  divide  value in AX by CX ( value C)
	ret 			     # return value in eax	
	.cfi_endproc
.LFE11:
	.size	asma, .-asma
	.ident	"GCC: (GNU) 5.3.0"
	.section	.note.GNU-stack,"",@progbits
