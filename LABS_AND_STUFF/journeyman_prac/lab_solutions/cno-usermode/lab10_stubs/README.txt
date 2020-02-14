Give students hooking.c, hooking.h, main.c, and Makefile.

Students must implement functions that use entry_stub_t type to perform a hook:
    -entry_stub_create
    -entry_stub_hook
    -entry_stub_unhook
    -ebtry_stub_free

Expected output:
This is a test of fputs.
Simon says: I've been hooked.
This should bypass the hook.
Hook has been removed.

notes:

Using
    movabs rax, addr
    jump rax

for trampolines etc because there's no 64 bit relative jump

This wouldn't worth with variable argument functions involving floating point regs,
because rax is used
