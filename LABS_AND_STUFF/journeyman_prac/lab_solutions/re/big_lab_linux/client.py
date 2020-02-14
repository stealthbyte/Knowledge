#!/usr/bin/python3

import socket

HOST = "localhost"
PORT = 8080

# this is a short example of connecting and communicating over a socket

s = socket.create_connection((HOST, PORT))

s.send(b"Hello!")
print(s.recv(0x400))

s.close()
