	.file	"gritty.c"
	.code16gcc
	.text
#APP
	call main
call exit

#NO_APP
	.local	curr_heap
	.comm	curr_heap,4,4
	.local	alloc_init
	.comm	alloc_init,1,1
	.globl	putchar
	.type	putchar, @function
putchar:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	8(%ebp), %eax
	movb	%al, -12(%ebp)
	movzbl	-12(%ebp), %eax
	subl	$12, %esp
	pushl	%eax
	call	xputchar
	addl	$16, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	putchar, .-putchar
	.globl	print
	.type	print, @function
print:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	cmpl	$0, 8(%ebp)
	je	.L8
	jmp	.L6
.L7:
	movl	8(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, 8(%ebp)
	movb	(%eax), %al
	movzbl	%al, %eax
	subl	$12, %esp
	pushl	%eax
	call	putchar
	addl	$16, %esp
.L6:
	movl	8(%ebp), %eax
	movb	(%eax), %al
	testb	%al, %al
	jne	.L7
	jmp	.L3
.L8:
	nop
.L3:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	print, .-print
	.globl	alloc
	.type	alloc, @function
alloc:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$20, %esp
	movl	8(%ebp), %eax
	movw	%ax, -20(%ebp)
	movb	alloc_init, %al
	testb	%al, %al
	jne	.L10
	movb	$1, alloc_init
	movl	$__heap, curr_heap
.L10:
	cmpw	$0, -20(%ebp)
	jne	.L11
	movl	$0, %eax
	jmp	.L12
.L11:
	movl	curr_heap, %eax
	movl	%eax, -4(%ebp)
	movl	curr_heap, %edx
	movzwl	-20(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, curr_heap
	movl	-4(%ebp), %eax
.L12:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	alloc, .-alloc
	.globl	freeall
	.type	freeall, @function
freeall:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	$__heap, curr_heap
	nop
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	freeall, .-freeall
	.section	.rodata
.LC0:
	.string	"alloc error"
	.text
	.globl	main
	.type	main, @function
main:
.LFB4:
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x7c,0x6
	subl	$20, %esp
	pushl	$5
	call	alloc
	addl	$4, %esp
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	.L16
	subl	$12, %esp
	pushl	$.LC0
	call	print
	addl	$16, %esp
	jmp	.L15
.L16:
	movl	-12(%ebp), %eax
	movb	$97, (%eax)
	movl	-12(%ebp), %eax
	incl	%eax
	movb	$98, (%eax)
	movl	-12(%ebp), %eax
	addl	$2, %eax
	movb	$99, (%eax)
	movl	-12(%ebp), %eax
	addl	$3, %eax
	movb	$100, (%eax)
	movl	-12(%ebp), %eax
	addl	$4, %eax
	movb	$0, (%eax)
	subl	$12, %esp
	pushl	-12(%ebp)
	call	print
	addl	$16, %esp
	nop
.L15:
	movl	-4(%ebp), %ecx
	.cfi_def_cfa 1, 0
	leave
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
