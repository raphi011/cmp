	.file	"test2.c"
	.text
	.globl	asma
	.type	asma, @function
asma:
.LFB0:
	.cfi_startproc
	movq	(%rdi), %r8
	movq	%r8, %rcx
	subq	(%rsi), %rcx
	movq	%rcx, (%rdx)
	movq	8(%rdi), %rax
	subq	8(%rsi), %rax
	cmpq	%rcx, %r8
	setb	%cl
	movzbl	%cl, %ecx
	subq	%rcx, %rax
	movq	%rax, 8(%rdx)
	ret
	.cfi_endproc
.LFE0:
	.size	asma, .-asma
	.ident	"GCC: (GNU) 4.9.2 20150304 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
