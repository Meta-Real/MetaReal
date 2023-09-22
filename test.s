.LC0:
	.ascii	"6624737266949237011120128\0"
	.globl	main
main:
	subq	$32, %rsp
	call	__main
	leaq	.LC0(%rip), %rcx
	call	puts
	xorl	%eax, %eax
	addq	$32, %rsp
	ret
