	.file	"asmb.c"
	.text
	.globl	asmb
	.type	asmb, @function
asmb:
.LFB0:
	.cfi_startproc
	movq	%rdi, %rax
	movq	%rdx, %r9
	testq	%rcx, %rcx
	je	.L2
	movl	$0, %r8d
.L3:
	imulq	(%rsi,%r8,8)
	idivq	(%r9,%r8,8)
	addq	$1, %r8
	cmpq	%r8, %rcx
	jne	.L3
.L2:
	ret
	.cfi_endproc
.LFE0:
	.size	asmb, .-asmb
	.ident	"GCC: (GNU) 5.3.0"
	.section	.note.GNU-stack,"",@progbits
