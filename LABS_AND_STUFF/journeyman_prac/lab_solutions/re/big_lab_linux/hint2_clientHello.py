#!/usr/bin/python3
import socket
import struct

HOST = "localhost"
PORT = 9876

def sendClientHello(s):
    hello = bytes.fromhex(
        "16"                        # content type: Handshake
        "03 01"                     # version
        "00 dc"                     # length
        "01"                        # handshake type: Client Hello
        "00 00 d8"                  # length
        "03 03"                     # version
        f"{'ff' * 0x20}"            # random
        "00"                        # session ID len
        "00 66"                     # cipher suites len
        "c0 14 c0 0a c0 22 c0 21"   # cipher suites
        "00 39 00 38 00 88 00 87"
        "c0 0f c0 05 00 35 00 84"
        "c0 12 c0 08 c0 1c c0 1b"
        "00 16 00 13 c0 0d c0 03"
        "00 0a c0 13 c0 09 c0 1f"
        "c0 1e 00 33 00 32 00 9a"
        "00 99 00 45 00 44 c0 0e"
        "c0 04 00 2f 00 96 00 41"
        "c0 11 c0 07 c0 0c c0 02"
        "00 05 00 04 00 15 00 12"
        "00 09 00 14 00 11 00 08"
        "00 06 00 03 00 ff"
        "01 00"                     # compression methonds (len 1, method null)
        "00 49"                     # extensions length
        "00 0b 00 04 03 00 01 02"   # ec_points_formats extension
        "00 0a 00 34 00 32 00 0e"   # supported_groups extension
        "00 0d 00 19 00 0b 00 0c"
        "00 18 00 09 00 0a 00 16"
        "00 17 00 08 00 06 00 07"
        "00 14 00 15 00 04 00 05"
        "00 12 00 13 00 01 00 02"
        "00 03 00 0f 00 10 00 11"
        "00 23 00 00"               # session_ticket extension
        "00 0f 00 01 01"            # heartbeat extension (peer allowed to send requests"
        
    )

    s.send(hello)
    return

def main():
    # create a connection to the server
    s = socket.create_connection((HOST, PORT))
    (s.recv(1024)

    # send the command to start tls
    s.send(b"4\n")

    # Do TLS stuff here:
    sendClientHello(s)
            

    s.close()

if __name__ == '__main__':
    main()
