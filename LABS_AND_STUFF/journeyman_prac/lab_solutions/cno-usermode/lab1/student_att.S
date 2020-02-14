.global xorprint
.global printTime

.section .data
adstring:
	.asciz "AD"
bcstring:
	.asciz "BC"
formatstring:
	.asciz "%02u/%02u/%04u %s %02u:%02u:%3.2f\n"

.section .text

/* void xorprint(int len, char key, char* ciphertext) */
.func xorprint
xorprint: 
	mov %rdx, %rcx /* put &ciphertext in rcx */
	xor %r8, %r8 /* zero r8 */
	mov %edi, %r8d /* put len in r8 */
xorprint_loop:
	dec %edi /* loop counter dec */
	jl xorprint_done
	xor %sil,(%rdx) /* xor key with ciphertext[i] */
	inc %rdx /* increment ciphertext pointer */
	jmp xorprint_loop
xorprint_done:
	/* call sys_write(stdout_fd, buf, count) */
	xor %rax, %rax /* clear rax */
	inc %rax /* rax = 1 */
	mov %rax, %rdi /* 1st arg for write() */
	mov %rcx, %rsi /* 2nd arg for write() */
	mov %r8, %rdx /* 3rd arg for write() */
	syscall
	ret
.endfunc

/* void printTime(int year, int month, int day, int hour, int minute, float second) */
.func printTime
printTime:
	push %r8		/* save minute */
	mov %rcx, %r9 /* put hour in r9 */
	mov %rdi, %rcx /* put year in rcx */
	lea (adstring), %r8 /* put AD string in r8 */
	test %ecx, %ecx
	jns printTime_1
	lea (bcstring), %r8
	neg %ecx
printTime_1:	
	/* rsi is already month, rdx is already day */
	lea (formatstring), %rdi
	/* rdi fmt, rsi month, rdx day, rcx year, r8 adstr, r9 hour, stack1 minute, xmm0 second */
	/* convert float to double */
	cvtss2sd %xmm0, %xmm0
	/* rax hold number of xmm reg used */
	movq $1, %rax
	call printf
	/* clear pushed arg */
	pop %rcx
	ret
.endfunc
