import pickle
import random
import socket
import struct
import sys
import threading
from datetime import datetime


def handle_connection(sock, remote_addr):
    try:
        num1 = random.randint(0, 0xFFFF)
        num2 = random.randint(0, 0xFFFF)
        endianness = b'<' if random.randint(0, 1) else b'>'
        # endianness = random.choice(b'<>')
        packed_nums = struct.pack("%scLL" % endianness.decode(), endianness, num1, num2)
        sock.sendall(pickle.dumps(packed_nums))
        client_data = sock.recv(100)  # arbitrarily large buffer
        client_sum = struct.unpack("%sL" % endianness.decode(), pickle.loads(client_data))[0]  # Returns a tuple
        print("%s - Sent %d and %d, received: %d" % (datetime.now(), num1, num2, client_sum))

        if client_sum == (num1 + num2):
            sock.sendall(b"CORRECT! Darth Vader is Luke's father!")
            print("%s - %s got the SECRET KEY!" % (datetime.now(), str(remote_addr)))
        else:
            sock.sendall(b"INCORRECT! Terminating connection!")
            print("%s - %s was denied!" % (datetime.now(), str(remote_addr)))
    except:
        print(sys.exc_info())
        print("%s - EXCEPTION! %s was denied!" % (datetime.now(), str(remote_addr)))
    finally:
        print()
        sock.close()


# IPv4, TCP by default
sock = socket.socket()
sock.bind(('', 3333))
sock.listen(5)

try:
    while True:
        new_sock, remote_addr = sock.accept()
        print("%s - Got a new connection from %s" % (datetime.now(), str(remote_addr)))
        threading.Thread(target=handle_connection,
                         args=(new_sock, remote_addr)).start()
finally:
    sock.close()
