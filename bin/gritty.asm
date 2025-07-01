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
	.globl	video_mode
	.type	video_mode, @function
video_mode:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	8(%ebp), %eax
	movb	%al, -12(%ebp)
	cmpb	$13, -12(%ebp)
	ja	.L6
	movzbl	-12(%ebp), %eax
	subl	$12, %esp
	pushl	%eax
	call	xvideo_mode
	addl	$16, %esp
	jmp	.L3
.L6:
	nop
.L3:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	video_mode, .-video_mode
	.globl	getchar
	.type	getchar, @function
getchar:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	call	xgetchar
	movzbl	%al, %eax
	movw	%ax, -10(%ebp)
	movw	-10(%ebp), %ax
	movb	%al, -11(%ebp)
	movb	$0, -12(%ebp)
	cmpb	$0, -11(%ebp)
	je	.L8
	movb	-11(%ebp), %al
	jmp	.L9
.L8:
	movb	-12(%ebp), %al
.L9:
	movb	%al, -13(%ebp)
	movb	-13(%ebp), %al
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	getchar, .-getchar
	.globl	print
	.type	print, @function
print:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	cmpl	$0, 8(%ebp)
	je	.L16
	jmp	.L14
.L15:
	movl	8(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, 8(%ebp)
	movb	(%eax), %al
	movzbl	%al, %eax
	subl	$12, %esp
	pushl	%eax
	call	putchar
	addl	$16, %esp
.L14:
	movl	8(%ebp), %eax
	movb	(%eax), %al
	testb	%al, %al
	jne	.L15
	jmp	.L11
.L16:
	nop
.L11:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	print, .-print
	.globl	alloc
	.type	alloc, @function
alloc:
.LFB4:
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
	jne	.L18
	movb	$1, alloc_init
	movl	$__heap, curr_heap
.L18:
	cmpw	$0, -20(%ebp)
	jne	.L19
	movl	$0, %eax
	jmp	.L20
.L19:
	movl	curr_heap, %eax
	movl	%eax, -4(%ebp)
	movl	curr_heap, %edx
	movzwl	-20(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, curr_heap
	movl	-4(%ebp), %eax
.L20:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	alloc, .-alloc
	.globl	freeall
	.type	freeall, @function
freeall:
.LFB5:
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
.LFE5:
	.size	freeall, .-freeall
	.globl	main
	.type	main, @function
main:
.LFB6:
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
	subl	$12, %esp
	pushl	$0
	call	video_mode
	addl	$16, %esp
	call	getchar
	movb	%al, -9(%ebp)
	nop
	movl	-4(%ebp), %ecx
	.cfi_def_cfa 1, 0
	leave
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
