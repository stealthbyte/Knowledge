Give students hooking.c, hooking.h, main.c, and Makefile.

Students must implement deref_jump, write_jump, and patch_code in hooking.c, and then succesfully call them from main.c

Expected output:
Calling func1 for the first time
func1 was called with 42
Calling return patched version of func1
Calling jump patched version of func1
func2 was called with 42

notes:

Using
    movabs rax, addr
    jump rax

for trampolines etc because there's no 64 bit relative jump

This wouldn't worth with variable argument functions involving floating point regs,
because rax is used
