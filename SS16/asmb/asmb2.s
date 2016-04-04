	.file	"asmb.c"
	.text
	.globl	asmb
	.type	asmb, @function
asmb:
.LFB0:
	.cfi_startproc
	pushq	%rbp                # push base pointer 
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp          # move stackpointer to basepointer
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)     # a
	movq	%rsi, -32(%rbp)     # b
	movq	%rdx, -40(%rbp)     # c
	movq	%rcx, -48(%rbp)     # n
	movq	$0, -8(%rbp)        # i = 0
	jmp	.L2
.L3:                                # for loop
	movq	-8(%rbp), %rax      # mov i to %rax
	leaq	0(,%rax,8), %rdx    # load effective address
	movq	-32(%rbp), %rax     # move b to %rax
	addq	%rdx, %rax          # calculate address of b[i] and put into %rax 
	movq	(%rax), %rax        # move value of address located in (%rax) to %rax 
	imulq	-24(%rbp)           # multiply a with b[i]
	movq	-8(%rbp), %rdx      # move i to %rdx
	leaq	0(,%rdx,8), %rcx    # 
	movq	-40(%rbp), %rdx     #
	addq	%rcx, %rdx          # 
	movq	(%rdx), %rsi        # 
	idivq	%rsi                # divide result of (a*b[i]) by c[i]
	movq	%rax, -24(%rbp)     # and assign a with the end result
	addq	$1, -8(%rbp)        # i++
.L2:
	movq	-8(%rbp), %rax
	cmpq	-48(%rbp), %rax     # compare i with n
	jb	.L3                 # if i < n jmp to L3 ( loop )
	movq	-24(%rbp), %rax
	popq	%rbp                #  return a
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	asmb, .-asmb
	.ident	"GCC: (GNU) 5.3.0"
	.section	.note.GNU-stack,"",@progbits
