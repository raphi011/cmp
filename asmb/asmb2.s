	.file	"asmb2.c"
	.text
	.globl	asmb
        .type	asmb, @function
asmb:
.LFB0:
	.cfi_startproc
	testq	%rcx, %rcx
	je	.END
	movl	$0, %eax
	movl	$0, %r9d    ; se
        clc                 ; clear carry flag
.LOOP:
        movq    (%rdi, %rax, 8), %r10
        sbbq    (%rsi, %rax, 8), %r10   ; x[i] - y[i] - carry
        movq    %r10, (%rdx, %rax, 8)   ; ergebnis r[i] zuweisen 
        addq    %1, %rax                ; i++
        cmpq    %rcx, %rax              ; i == n?
        jne     .LOOP                   ; if not : continue
.END
        sbbq    %r9 %r9                 ; negated carry value
        movq    %r9, (%rdx, %rax, 8)    ; r[n] = carry
	ret
	.cfi_endproc
.LFE0:
	.size	asmb, .-asmb
	.ident	"GCC: (GNU) 4.9.2 20150304 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
