; nasm -f elf64 -o hello.o hello_world.nasm
; ld -o hello hello.o
; ./hello

global _start

section .text

_start:
  mov rax, 0x4      ; write(
  mov rdi, 0x1      ;   STDOUT_FILENO,
  mov rsi, msg      ;   "Hello, world!\n",
  mov rdx, msglen   ;   sizeof("Hello, world!")
  ; syscall           ; );
  int 80h           ; );

  xor eax, eax
  mov rax, 0x1      ; exit(
  mov rdi, 0x0      ;   EXIT_SUCCESS
  ; syscall           ; );
  int 80h           ; );

section .rodata
  msg: db "Hello, world!"
  msglen: equ $ - msg
