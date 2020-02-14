import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(('127.0.0.1', 50000)) # bind to an addr and a port
sock.listen(1)
sock_to_client, remote_address = sock.accept()
print(remote_address)
sock_to_client.sendall(b'The world says hi.')
sock_to_client.close()
sock.close()