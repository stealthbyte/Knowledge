;;  _   _  ____ _______ ______
;; | \ | |/ __ \__   __|  ____|
;; |  \| | |  | | | |  | |__
;; | . ` | |  | | | |  |  __|
;; | |\  | |__| | | |  | |____
;; |_| \_|\____/  |_|  |______|  : Use ONLY the opcodes that have been taught so far in the class
;;
;; * Instruction provided solutions are often *NOT* the smallest way to solve the problem.  Be creative


BITS 32         ;you must specify bits mode
segment .text   ;you must specify a section


GLOBAL lab0_1, labSize0_1 ; this tells nasm to make the 2 names global so they can be used externally (ie: by the grader/validation code)


lab0_1:
; Lab0.1
; GOAL:
;   Put a breakpoint in the code
; STEPS:
;   Run this with Ctrl+Shift+B, then choose build-linux
;;;;;;;;;;;;; YOUR CODE BELOW
	;int3
;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize0_1 dd $-lab0_1 -1


GLOBAL lab1_1, labSize1_1
lab1_1:
; Lab1.1:
; GOAL:
;   Exchange values in registers EAX and EBX
;   PRESERVE: nothing
;       Do -NOT- use xchg
;;;;;;;;;;;;; YOUR CODE BELOW
	mov edx, ebx
	mov ebx, eax
	mov eax, edx
;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize1_1 dd $-lab1_1 -1


GLOBAL lab1_2, labSize1_2
lab1_2:
; Lab1.2:
; GOAL:
;   Perform this action:
;     EAX = EBX + ECX + 0x42
;   PRESERVE: EBX, ECX, EDX
;;;;;;;;;;;;; YOUR CODE BELOW
	mov eax, 0x42
	add eax, ebx
	add eax, ecx
;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize1_2 dd $-lab1_2 -1


GLOBAL lab1_3, labSize1_3
lab1_3:
; Lab1.3:
;
; NEW INSTRUCTION: xchg <reg>, <reg>
;  -exchanges the values in registers of op1 and op2
;
; GOAL:
;   Exchange values in registers EAX and EBX
;   PRESERVE: ECX, EDX
;
; Can you accomplish this lab with the restrictions
; WITHOUT using xchg?
;
;;;;;;;;;;;;; YOUR CODE BELOW
	xchg eax, ebx
;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize1_3 dd $-lab1_3 -1


GLOBAL lab2_1, labSize2_1
lab2_1:
; Lab2.1:
;
; NEW INSTRUCTION: jmp <LABEL>
;
; GOAL:
;   Use a single jmp instruction to create an infinite loop.
;   PRESERVE: all registers
;
;;;;;;;;;;;;; YOUR CODE BELOW
	jmp lab2_1
;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize2_1 dd $-lab2_1 -1


GLOBAL lab2_2, labSize2_2
lab2_2:
; Lab2.2:
;
; NEW INSTRUCTIONS: je <LABEL>; jne <LABEL>; cmp <op1>,<op2>
;
; GOAL: Create assembly version of this pseudo-code
;   if( EAX == 42 )
;      EBX = 1
;   else
;      EBX = 2
;
;   if( ECX == 42 )
;      EDX = 1
;   else
;      EDX = 2
;
;   PRESERVE: EAX, ECX
;
;  Coding Convention:
;     Put spaces BEFORE each branch target and AFTER each BRANCH
;
;    BAD:
;       add bla, bla
;    .copyLoop:
;       cmp bla bla
;       jne .copyLoop
;       mov bla bla
;
;    GOOD:
;       add bla, bla
;
;    .copyLoop:
;       cmp bla bla
;       jne .copyLoop
;
;       mov bla bla
;
;
;    BONUS: Use the minimum number of instructions!
;    NOTE: That is a DECIMAL 42, not hex 42.
;;;;;;;;;;;;; YOUR CODE BELOW
		cmp eax, 42
		jne .eax_not_equal
		mov ebx, 1
		jmp .continue

	.eax_not_equal:
		mov ebx, 2

	.continue:
		cmp ecx, 42
		jne .ecx_not_equal
		mov edx, 1
		jmp .end

	.ecx_not_equal:
		mov edx, 2

	.end:

;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize2_2 dd $-lab2_2 -1


GLOBAL lab2_3, labSize2_3
lab2_3:
; Lab2.3:
;
; GOAL:
;   Initialize ECX to 0
;   Loop, adding 5 to ECX each time.
;   When ECX == EAX, exit the loop.
;   NOTE: only check ECX==EAX after the first
;    pass through the loop body. This is equivalent
;    to a 'do' loop in C:
;   do {
;     ECX += 5
;   } while( !(ECX==EAX) );
;
;  PRESERVE: EAX, EBX, EDX
;
;  The verifier should only take a split second to run your loop:
;   if it is more than a second or two, something is wrong!
;
;;;;;;;;;;;;; YOUR CODE BELOW
		xor ecx, ecx

	.loop:
		add ecx, 5
		cmp ecx, eax
		jne .loop


;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize2_3 dd $-lab2_3 -1


GLOBAL lab2_4, labSize2_4
lab2_4:
; Lab2.4:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;  BONUS LAB  ;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; GOAL:
;   Multiply 333 by 51 using only the instructions
;    discussed so far!
;   Store the result in EAX.
;
;   PRESERVE: EBX, EDX
;
;   HINT: you will only need one loop
;   HINT2: Once you've done it with one loop, can you "cheat" to make it smaller
;          WITHOUT using any opcodes not covered in the class (ie the multiply opcode)
;;;;;;;;;;;;; YOUR CODE BELOW
	xor eax, eax
	xor ecx, ecx
.loop:
	add eax, 333
	add ecx, 1
	cmp ecx, 51
	jne .loop

;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize2_4 dd $-lab2_4 -1


GLOBAL lab3_1, labSize3_1
lab3_1:
; Lab3.1:
;
; GOAL:
;   Exchange AL and AH
;   PRESERVE: EBX, ECX, EDX
;
;;;;;;;;;;;;; YOUR CODE BELOW
	xchg al, ah
;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize3_1 dd $-lab3_1 -1


GLOBAL lab3_2, labSize3_2
lab3_2:
; Lab3.2:
;
; GOAL:
;   Move upper 8-bits of EAX into DL
;   PRESERVE: EAX, EBX, ECX
;
;;;;;;;;;;;;; YOUR CODE BELOW
	mov edx, eax
	shr edx, 24
;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize3_2 dd $-lab3_2 -1


GLOBAL lab3_3, labSize3_3
lab3_3:
; Lab3.3:
;
; GOAL:
;   Set EAX equal to the value in AH
;   PRESERVE: EBX, ECX, EDX
;
;;;;;;;;;;;;; YOUR CODE BELOW
	movzx eax, ah
;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize3_3 dd $-lab3_3 -1


GLOBAL lab3_4, labSize3_4
lab3_4:
; Lab3.4:
;
; GOAL:
;   Divide EAX by 64 (integer division)
;   PRESERVE: EBX, ECX, EDX
;
;;;;;;;;;;;;; YOUR CODE BELOW
	shr eax, 6
;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize3_4 dd $-lab3_4 -1



GLOBAL lab3_5, labSize3_5
lab3_5:
; Lab3.5:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;  BONUS LAB  ;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; GOAL:
;   Convert EAX from host byte order to network byte order.
;   This will mean reversing the byte order.
;   For example, if EAX contained the value 0x87654321, you
;    would convert it to 0x21436587.
;
;   Try to minimize the number of instructions!
;
;   PRESERVE: ECX, EDX
;
;   BONUS if you preserve EBX as well!
;
;;;;;;;;;;;;; YOUR CODE BELOW
	xchg ah, al
	ror eax, 16
	xchg ah, al



;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize3_5 dd $-lab3_5 -1


GLOBAL lab4_1, labSize4_1
lab4_1:
; Lab4.1:
;
; GOAL: Exchange EAX and EBX using:
;	-ONLY MOV instructions
;   -the memory pointed to by ECX as temp storage for the swap
;
;   PRESERVE: ECX, EDX
;
;;;;;;;;;;;;; YOUR CODE BELOW

	mov [ecx], eax
	mov eax, ebx
	mov ebx, [ecx]

;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize4_1 dd $-lab4_1 -1


GLOBAL lab4_2, labSize4_2
lab4_2:
; Lab4.2:
;
; GOAL:
;   Set the dword pointed to by EAX to the value
;    of the byte pointed to by EBX.
;   PRESERVE: EAX, ECX, EDX, value pointed to by EBX
;
;;;;;;;;;;;;; YOUR CODE BELOW

	movzx ebx, byte [ebx]
	mov [eax], ebx

;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize4_2 dd $-lab4_2 -1


GLOBAL lab4_3, labSize4_3
lab4_3:
; Lab4.3:
;
; GOAL: Add ECX and EBX and store in EAX in one instruction.
;      EAX <- ECX + EBX
;
;   PRESERVE: EBX, ECX, EDX
;
;;;;;;;;;;;;; YOUR CODE BELOW

	lea eax, [ecx + ebx]

;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize4_3 dd $-lab4_3 -1


GLOBAL lab4_4, labSize4_4
lab4_4:
; Lab4.4:
;
; GOAL:
;   Fill the buffer pointed to by EAX with the byte-sized integers from 0-64 inclusive
;   So, a memory dump of EAX will look like: 00 01 02 03 04.....
;   PRESERVE: EAX, EBX, EDX
;
;;;;;;;;;;;;; YOUR CODE BELOW

	xor ecx, ecx

.loop_it:
	mov byte [eax + ecx], cl
	inc ecx
	cmp ecx, 64
	jle .loop_it

;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize4_4 dd $-lab4_4 -1


GLOBAL lab4_5, labSize4_5
lab4_5:
; Lab4.5:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;  BONUS LAB  ;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; GOAL:
;   EAX points to a NULL-terminated ASCII string.
;   EBX points to memory to which you will write a 16-bit integer
;    followed by the string without a NULL-terminator.
;   The 16-bit integer will be the string length.
;	eax -> "Some string"
;   ebx -> buf (should look like the following when done...)
;	----------------------------
;   |0B|Some string|
;	----------------------------
;       ^^^^^^^^^^^ - original string w/o NULL
;    ^^ - First 2 bytes are length of string
;   PRESERVE: EAX, EBX, Memory at EAX.
;
;;;;;;;;;;;;; YOUR CODE BELOW
	xor ecx, ecx

.str_len:
	cmp byte [eax + ecx], byte 0
	jz .write_str_len
	mov dl, byte [eax + ecx]
	mov byte [ebx + ecx + 2], dl
	inc ecx
	jmp .str_len

.write_str_len:
	mov word [ebx], cx


;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize4_5 dd $-lab4_5 -1


GLOBAL lab5_1, labSize5_1
lab5_1:
; Lab5.1:
;
; GOAL:
;   Exchange EAX and EBX using ONLY push and pop instructions
;
;   PRESERVE: all but EAX, EBX
;
;;;;;;;;;;;;; YOUR CODE BELOW
	push eax
	push ebx
	pop eax
	pop ebx
;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize5_1 dd $-lab5_1 -1


GLOBAL lab5_2, labSize5_2
lab5_2:
; Lab5.2:
;
; GOAL: Exchange EAX and EBX using ONLY pushad and pop
;
;   PRESERVE: all but EAX, EBX
;
;;;;;;;;;;;;; YOUR CODE BELOW

	pushad
    pop eax
	pop eax
	pop eax
	pop eax
	pop eax
	pop ebx
	pop ebx
	pop ebx

;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize5_2 dd $-lab5_2 -1


GLOBAL lab5_3, labSize5_3
lab5_3:
; Lab5.3:
;
; GOAL: Remove 0x800 bytes from the top of the stack.
;   NOTE: If you do it wrong, you'll likely crash!
;
;   PRESERVE: all but ESP
;
;
;;;;;;;;;;;;; YOUR CODE BELOW
	add esp, 0x800
;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize5_3 dd $-lab5_3 -1


GLOBAL lab5_4, labSize5_4
lab5_4:
; Lab5.4:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;  BONUS LAB  ;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; GOAL: Copy the the NULL-terminated ASCII string pointed to by EAX
;   to the buffer pointed to by EBX, including trailing NULL byte.
;   Store the string length in EAX.
;
;   PRESERVE: all registers but EAX
;
;;;;;;;;;;;;; YOUR CODE BELOW
	push ecx
	push edx
	xor ecx, ecx

.str_len:
	cmp byte [eax + ecx], byte 0
	jz .write_str_len
	mov dl, byte [eax + ecx]
	mov byte [ebx + ecx], dl
	inc ecx
	jmp .str_len

.write_str_len:
	mov [ebx + ecx], byte 0
	mov eax, ecx
	pop edx
	pop ecx

;;;;;;;;;;;;; YOUR CODE ABOVE
	ret
labSize5_4 dd $-lab5_4 -1


GLOBAL lab6_1, labSize6_1
lab6_1:
; Lab6.1:
;
; GOAL:
;   Implement entire function:
;	 int _stdcall getStrLen(const char *sz);
;
;   Return the length of the NULL-terminated ASCII string.
;   Do NOT use EBP, ESI, or EDI or previously unintroduced instructions.
;
;   (NOTE: Do not be confused by the lab's label: if it were truly called
;   from C as a extern _stdcall function, it's name would be different
;   (_lab6_1@4). The verifier imports EVERY lab as 'void _cdecl *(void)'
;   type, but then calls it raw. )
;
;   PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

	mov edx, [esp+4]		;p1
	xor eax, eax

.str_len:
	cmp byte [edx + eax], byte 0
	jz .done_str_len
	inc eax
	jmp .str_len

.done_str_len:
	ret 4

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize6_1 dd $-lab6_1


GLOBAL lab6_2, labSize6_2
lab6_2:
; Lab6.2:
;
; GOAL:
;   Implement entire function:
;	 int _fastcall addThree(int a, int b, int c);
;
;   Add the three arguments together and return the result.
;
;   PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

	lea eax, [ecx + edx]		;p1, p2
	add eax, [esp+4]			;p3
	ret 4

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize6_2 dd $-lab6_2


GLOBAL lab6_3, labSize6_3
lab6_3:
; Lab6.3:
;
; GOAL:
;   Implement entire function:
;	 char *_stdcall copyString(const char *sz);
;
;   Return a copy of the null-terminated string by calling the
;    following functions:
;
;        void * _cdecl  _my_malloc(int size); //allocates heap buffer of specified size
;        int _stdcall   _my_strlen@4(const char *str); //gets length of null-terminated string not including terminator
;        void _fastcall @my_memcpy@12(void *dst, const void *src, int size); //copies memory
;
;   NOTES:
;     You MUST call each of these functions, and no others.
;     You MUST NOT have any loops in your function.
;     You may assume that my_malloc will not fail.
;     The lab validation code will attempt to free the returned buffer.
;     Assume all the called functions preserve standard Windows registers.
;     DO NOT forget to include to copy the string terminator!
;     You MAY use ESI and EDI as general registers.
;
;   CONVENTION: Put spaces between before and after a chunk of code associated
;               with a function call such as pushing parameters, saving registers
;               or restoring stack
;   PRESERVE: standard Windows registers
;
extern my_malloc
extern my_strlen
extern my_memcpy

;;;;;;;;;;;;; YOUR CODE BELOW

	;preserve regs

	push ebx
	push esi
	push edi

	
	mov esi, [esp+16]	;src str to esi	sub esp, 4
	push esi			;push src str to stack for call esp+4
	call my_strlen		;callee cleaned, eax holds len

	mov ebx, eax		;store len into ecx
	add ebx, 1			; may need to add 1 for null term
	push ebx			;push len to stack for next call,
	call my_malloc		;caller cleaned, eax holds pointer to buffer
	add esp, 4			;cleaned

	mov edi, eax		;buf is now edi

	mov edx, esi		;store src str, p2, in edx for fastcall
	mov ecx, edi		;store buf, p1, in ecx for fastcall
	push ebx			;src str len, p3

	call my_memcpy

	;restore preserved regs
	pop edi				;pop preserved value lin 643
	pop esi				;pop preserved value lin 642
	pop ebx				;pop preserved value lin 641

	ret 4				;ret 4 for cleanup


;;;;;;;;;;;;; YOUR CODE ABOVE
labSize6_3 dd $-lab6_3


GLOBAL lab6_4, labSize6_4
lab6_4:
; Lab6.4:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;  BONUS LAB  ;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; GOAL:
;   Implement entire function:
;	 ULONG _cdecl addNumbers(ULONG count, ...);
;
;   Arguments consist of a count then that many unsigned integers.
;   Add all the arguments after the count, and return the result.
;
; Restriction:
;   Don't use magic numbers.
;   BAD:
;       add ecx, 4
;   GOOD:
;       %define DWORD_SIZE	4
;       add   ecx, DWORD_SIZE
;
;   PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

%define DWORD_SIZE	4

	mov ecx, [esp+DWORD_SIZE]
	xor eax, eax

	.add_loop:
	cmp ecx, 0
	je .done
	add eax, [esp+DWORD_SIZE + ecx*DWORD_SIZE]
	Loop .add_loop

	.done:
	ret

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize6_4 dd $-lab6_4


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;  BONUS LAB  ;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GLOBAL lab6_5, labSize6_5
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; GOAL:
;   Call the C library function:
;	int printf(const char *format, ...)
;   and print out the string "Assembly rocks!\n"
;
;;;;;;;;;;;;; YOUR CODE BELOW
extern printf
extern exit
myString db "Assembly rocks!", 0x0a, 0x00
lab6_5:
push myString
call printf
add esp, 4
push 0
call exit

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize6_5 dd $-lab6_5


GLOBAL lab7_1, labSize7_1
lab7_1:
; Lab7.1:

; GOAL:
;   Implement internals of a function:
;   NOTE! The entry and exit of the function
;    have been PROVIDED (outside of the YOUR CODE sections)
;
;   USE hardcoded integer offsets from EBP to access arguments.
;
;   ULONG _stdcall addNumbers(ULONG a, ULONG b);
;
;    Adds a and b and returns result
;
;   PRESERVE: standard Windows registers
;
    push ebp
    mov ebp, esp
;;;;;;;;;;;;; YOUR CODE BELOW

	mov eax, [ebp+8]
	add eax, [ebp+12]

;;;;;;;;;;;;; YOUR CODE ABOVE
    pop ebp
    ret 8

labSize7_1 dd $-lab7_1


GLOBAL lab7_2, labSize7_2
lab7_2:
; Lab7.2:
;
; GOAL:
;   Access arguments using stack frame base pointer (EBP) and a struc.
;
;   Define your struc within the YOUR CODE section, name it 'Args72'
;
;     Alternatively, use %define to declare the location of your locals
;
;   You will implement entire function.
;     int _stdcall my_strncmp(const char *szA, const char *szB, int maxLength);
;
;   Similar to but different than standard C strncmp:
;      You will return 0 if strings match.
;      You will return 1 if strings do NOT match.
;      Returns a result when either:
;         -maxLength characters have been examined
;         -the first difference between the strings is found
;         -a null terminator is reached in either string
;
;
;    Hint: check maxLength, THEN check char, THEN check for null terminator
;
;    TEST CASES:
;         my_strncmp("","",0) == 0
;         my_strncmp("","",1) == 0
;         my_strncmp("X","",1) == 1
;         my_strncmp("X","Y",1) == 1
;         my_strncmp("X","Y",0) == 0
;         my_strncmp("XX","XY",1) == 0
;         my_strncmp("XX","XY",2) == 1
;         my_strncmp("XX","XXX",2) == 0
;         my_strncmp("XX","XXX",3) == 1
;         my_strncmp("XX","XXX",2000); == 1
;
;    In this lab especially, do not rely soley on the verifier!
;    If you did not use EBP right (or used disallowed instructions),
;     the verifier may still verify this but you will fail the lab when graded!
;
;   PRESERVE: standard Windows registers
;
;
;;;;;;;;;;;;; YOUR CODE BELOW

    push ebp
    mov ebp, esp

	push esi
	push ebx

	; Load Arguments
	mov eax, [ebp+Args72.szA]
	mov ebx, [ebp+Args72.szB]
	mov esi, [ebp+Args72.maxLength]

	.check_zeroLen:
	cmp esi, 0
	je .set_0
	
	.check_char:
	mov dl, [eax]
	mov dh, [ebx]
	inc eax
	inc ebx
	cmp dl, dh
	jne .set_1

	.check_length:
	dec esi
	cmp esi, 0
	je .set_0

	.check_null:
	cmp dl, 0
	je .set_0
	jmp .check_char

	.set_1:
	xor eax, eax
	inc eax
	jmp .done

	.set_0:
	xor eax, eax

	.done:
	pop ebx
	pop esi
	pop ebp
    ret 12

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize7_2 dd $-lab7_2


GLOBAL lab7_3, labSize7_3
lab7_3:
; Lab7.3:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;  BONUS LAB  ;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; GOAL:
;   Reimplement Lab 6.3 (allocating copy of string), using
;    local variables in some way.
;
;   As helpful motivation to use locals, you may NOT use EBX, ESI, or EDI,
;     even if you store and restore them!
;
;   Use EBP to access both arguments and locals, in whichever manner you see fit.
;
;   PRESERVE: standard Windows registers
;
;   Did you know??
;     You can use enter and leave opcodes in place of "push ebp/mov ebp, esp".
;     Try it for this lab
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize7_3 dd $-lab7_3


GLOBAL lab7_4, labSize7_4
lab7_4:
; Lab7.4:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;  BONUS LAB  ;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; GOAL:
;    print the return addresses on the callstack to your function using printf
;    Use the "k" command in WinDbg to verify that your code is working properly
;     PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize7_4 dd $-lab7_4


GLOBAL lab8_1, labSize8_1
lab8_1:
; Lab8.1:
;
; GOAL:
;   Implement entire function:
;      void _cdecl memcpy(void *dst, void *src, int count);
;
;   You MUST use lodsb and stosb!
;
;   PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize8_1 dd $-lab8_1


GLOBAL lab8_2, labSize8_2
lab8_2:
; Lab8.2:
;
; GOAL:
;   Implement entire function:
;      void _cdecl memcpy(void *dst, void *src, int count);
;
;   You MUST use rep movsb!
;
;   PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize8_2 dd $-lab8_2



GLOBAL lab8_3, labSize8_3
lab8_3:
; Lab8.3:

; GOAL:
;   Implement entire function:
;        void _stdcall memset(void *buf, int c, int count);
;
;   You MUST use rep stosb!
;
;   Hint: Nearly identical to Lab 8.2!
;
;   PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize8_3 dd $-lab8_3


GLOBAL lab8_4, labSize8_4
lab8_4:
; Lab8.4:

; GOAL:
;   Implement entire function:
;        void _fastcall slideUp(void *dst, void *src, int size);
;
;   You must use rep movsb.
;   Buffers are overlapping and src is lower than dst: you will need
;    to copy from top down instead of bottom up.
;   If you set DF, don't forget to clear it before returning.
;
;   PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize8_4 dd $-lab8_4


GLOBAL lab8_5, labSize8_5
lab8_5:
	struc Args72
		.old_ebp	resd	1
		.old_eip	resd	1
		.szA		resd	1
		.szB		resd	1
		.maxLength	resd	1
	endstruc
; Lab8.5:
;
; GOAL:
;   Re-Implement Lab 7.2 using repe or repne and cmpsb
;   The arg access and frame stuff is optional (if you do reuse it,
;    feel free to reuse the Arg72 structure without defining).
;
;   You will need to test the termination condition of the cmpsb
;    to determine if it exited because of mismatch or ECX==0 (try JNE).
;
;   PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize8_5 dd $-lab8_5


GLOBAL lab8_6, labSize8_6
lab8_6:
; Lab8.6:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;  BONUS LAB  ;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; GOAL: ;
;   Implement the entire function:
;	 const char *_stdcall my_memchr(void *buf, char ch, int count);
;
;   Return NULL if ch is not present in buf within count bytes.
;   Return pointer to ch otherwise.
;
;   You MUST use scasb either in a loop or with a repe OR repne prefix.
;   HINT: one form is easier...
;   You will need to test for whether it found it or exited because it
;    ran out of bytes.
;
;   PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize8_6 dd $-lab8_6


GLOBAL lab9_1, labSize9_1
lab9_1:
; Lab9_1:
;
; GOAL: ;
;
;	Return with ZF true.
;
;   PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
ret
labSize9_1 dd $-lab9_1


GLOBAL lab9_2, labSize9_2
lab9_2:
; Lab9_2:
;
; GOAL: ;
;
;	CF and OF are almost never set at the same time.
;	Find a way to return with CF and OF both set.
;
;   PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
ret
labSize9_2 dd $-lab9_2


GLOBAL lab10_1, labSize10_1
lab10_1:
; Lab10_1:
;
; GOAL: ;
;
;	Implement entire function:
;		int _fastcall my_divider(uint dividend, uint divisor);
;
;	Return Quotient.
;
;
;   PRESERVE: standard Windows registers
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE

labSize10_1 dd $-lab10_1


GLOBAL lab10_2, labSize10_2
lab10_2:
; Lab10_2:
;
; GOAL: ;
;
;	Set EAX to 0.
;	test and complement bit 0 of ECX, if it was 1 increment EAX
;	test and complement bit 1 of ECX, if it was 0 increment EAX
;	test and reset bit 2 of ECX, if it was 1 increment EAX
;	test and reset bit 3 of ECX, if it was 0 increment EAX
;	test bit 4 of ECX, if it is 0 increment EAX
;	test bit 5 of ECX, if it is 1 increment EAX
;	if bit 6 of ECX is 1, set EDX to 2, else set EDX to 0 (do NOT use a jump!)
;
;
;   PRESERVE: standard Windows registers
;             Make only the designated changes to ECX
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
ret
labSize10_2 dd $-lab10_2


GLOBAL lab11_1, labSize11_1
lab11_1:
; Challenge 1
; Write a stdcall function that takes a single argument (N)
; and returns the Nth fibonacci number. Assume the sequence starts at 0,
; and is 1-based (that is, the first number is 0, the second is 1, and there is no 0th number).
;  * This can be recursive or non-recursive. If you're ambitious, write both
;
;
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
ret 4
labSize11_1 dd $-lab11_1


GLOBAL lab11_2, labSize11_2
lab11_2:
; Challenge 2
; Write a stdcall function that takes a single argument (N)
; and returns the sum of the natural numbers from 0 to N
; that are multiples of both 3 and 5
;  * Bonus if you only use add/sub (no mul/divide)
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
ret 4
labSize11_2 dd $-lab11_2


GLOBAL lab11_3, labSize11_3
lab11_3:
; Challenge 3
; Write the stdcall function
; stdcall int largestProduct(ULONG arrayLen, LONG *array)
; Return the largest product of five consecutive integers in the array.
;  * Worded another way: Find the largest product of five consecutive integers within an N-length array of integers.
; On error return -1
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
ret 8
labSize11_3 dd $-lab11_3


GLOBAL lab11_4, labSize11_4
lab11_4:
; Challenge 4
; Write the stdcall function
;	void _stdcall xorEncDec(char *inOutStream, unsigned char key, unsigned long len)
; This is an in-place xor encoder/decoder. First use string instructions, then implement it without.
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize11_4 dd $-lab11_4


GLOBAL lab11_5, labSize11_5
lab11_5:
; Write the stdcall function
;	char * _stdcall addCommas(char *numberString, unsigned long len)
; numberString is a string representing a number (e.g. "1234567")
; return a new string containing that number with commas inserted (e.g. "1,234,567").
; You may use any of the functions given to you in previous labs.
; For convenience, the ascii representation of "," is 0x2c
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize11_5 dd $-lab11_5


GLOBAL lab11_6, labSize11_6
lab11_6:
; Modify your solution to the Fibonacci problem to return the sum of even Fibonacci
; numbers up to the input number.
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize11_6 dd $-lab11_6

GLOBAL lab11_7, labSize11_7
lab11_7:
; Write the stdcall function
;	BOOL _stdcall validatePassword(char *password, unsigned long len)
; This function should return 1 if the password is valid, 0 otherwise.
;
; Passwords must begin with an alphabetic character [a-zA-Z], contain at
; least one number [0-9], at least one symbol [!@#$%^&*()], at least one
; uppercase alphabetic character [A-Z], and at least one lowercase alphabetic
; character [a-z].
; For convenience
; Ascii value list: [a-z] are 0x61-0x7a, [A-Z] are 0x41-0x5a, [0-9] are 0x30-0x39.
; [!, @, #, $, %, ^, &, *, (, )] is [0x21, 0x40, 0x23, 0x24, 0x25, 0x5e, 0x26, 0x2a, 0x28, 0x29]
;;;;;;;;;;;;; YOUR CODE BELOW

;;;;;;;;;;;;; YOUR CODE ABOVE
labSize11_7 dd $-lab11_7
