	.file	"asmb.c"
	.text
	.globl	asmb
	.type	asmb, @function
asmb:
.LFB0:
	.cfi_startproc
	testq	%rcx, %rcx
	je	.END
	movl	$0, %eax
	movl	$0, %r9d
        clc            
.LOOP:
        movq    (%rdi, %rax, 8), %r10
        sbbq    (%rsi, %rax, 8), %r10   
        movq    %r10, (%rdx, %rax, 8)  
        addq    $1, %rax              
        cmpq    %rcx, %rax           
        jne     .LOOP                 
.END:
        sbbq    %r9, %r9               
        movq    %r9, (%rdx, %rax, 8) 
	ret
	.cfi_endproc
.LFE0:
	.size	asmb, .-asmb
	.ident	"GCC: (GNU) 4.9.2 20150304 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
