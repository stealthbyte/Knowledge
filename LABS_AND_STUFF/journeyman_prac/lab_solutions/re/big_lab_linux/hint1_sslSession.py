#!/usr/bin/python3
import socket
import struct
import ssl

HOST = "localhost"
PORT = 9876

def doSSL(s):
    # do a regular ssl wrapping
    ctx = ssl.SSLContext(protocol=ssl.PROTOCOL_TLS)
    ctx.cerify_mode = ssl.CERT_OPTIONAL

    ssk = ctx.wrap_socket(s)

    # read a few bytes from the server
    for _ in range(9):
        ssk.recv(100)
        if len(inp) == 0:
            break
    
def main():
    # create a connection to the server
    s = socket.create_connection((HOST, PORT))
    s.recv(1024)

    # send the command to start tls
    s.send(b"4\n")

    # Do TLS stuff here:
    regularSSL(s)

    s.close()

if __name__ == '__main__':
    main()
