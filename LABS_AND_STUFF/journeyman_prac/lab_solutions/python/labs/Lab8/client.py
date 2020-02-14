import pickle
import socket
import struct

# IPv4, TCP by default
sock = socket.socket()

try:
    sock.connect(("localhost", 3333))
    pickled_data = sock.recv(100)  # arbitrarily large buffer
    data = pickle.loads(pickled_data)
    endianness = data[0].to_bytes(1, byteorder="big")
    
    print("Endianness: %s" % endianness.decode())
    
    num1 = struct.unpack("%sL" % endianness.decode(), data[1:5])[0]  # Returns a tuple

    num2 = struct.unpack("%sL" % endianness.decode(), data[5:9])[0]  # Returns a tuple

    print("Received %d and %d" % (num1, num2))

    total = num1 + num2

    print("Sending %d" % total)
    
    sum_packed = struct.pack("%sL" % endianness.decode(), total)
    sock.sendall(pickle.dumps(sum_packed))

    # Receive secret message
    msg = sock.recv(100)  # arbitrarily large buffer
    print(msg.decode("utf8"))
finally:
    sock.close()
