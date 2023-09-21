.LC0:
	.ascii	"[]\0"
.LC1:
	.ascii	"[1, 2, 3]\0"
	.globl	main
main:
	subq	$32, %rsp
	call	__main
	leaq	.LC0(%rip), %rcx
	call	puts
	leaq	.LC0(%rip), %rcx
	call	puts
	leaq	.LC1(%rip), %rcx
	call	puts
	leaq	.LC0(%rip), %rcx
	call	puts
	leaq	.LC1(%rip), %rcx
	call	puts
	xorl	%eax, %eax
	addq	$32, %rsp
	ret
