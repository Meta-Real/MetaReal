.LC0:
	.ascii	"12\0"
.LC1:
	.ascii	"12333\0"
.LC2:
	.ascii	"[12333, 12333]\0"
	.globl	main
main:
	subq	$32, %rsp
	call	__main
	leaq	.LC0(%rip), %rcx
	call	puts
	leaq	.LC1(%rip), %rcx
	call	puts
	leaq	.LC2(%rip), %rcx
	call	puts
	xorl	%eax, %eax
	addq	$32, %rsp
	ret
