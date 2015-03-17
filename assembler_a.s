	.file	"test.c"
	.text
	.globl	asma
	.type	asma, @function
asma:
.LFB0:
	.cfi_startproc
	movq	(%rdi), %rax
	subq	(%rsi), %rax
	movq	%rax, (%rdx)
        movq    8(%rdi), %rax
        sbbq    8(%rsi), %rax
        movq    %rax, 8(%rdx)
	ret
	.cfi_endproc
.LFE0:
	.size	asma, .-asma
	.ident	"GCC: (GNU) 4.9.2 20150304 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
