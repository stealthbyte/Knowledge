import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # create a socket

remote_address = ('127.0.0.1', 50000) # addr/port of remote service

# This won't work unless there is something listening at the above addr/port
sock.connect(remote_address)
sock.sendall(b'Hello world!\n') # string must be sent as bytes
print(sock.recv(100).decode()) # listen for the server's response, up to 100 bytes long
sock.close()