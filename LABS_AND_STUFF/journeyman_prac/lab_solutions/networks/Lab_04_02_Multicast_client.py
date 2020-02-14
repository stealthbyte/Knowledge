#!/user/bin/python3.7
import socket
import time
import struct

###########################################################
#
#   Lab 4 Task 2 - Multicast Client
#
#       Client that can send UDP datagrams on
#       Multicast address
#
###########################################################

# Configure address info for connection
dst_ip = "FF02::666:666:666"
dst_port = 7777
# Create and configure UDP socket (DGRAM)

my_sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
my_sock.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_MULTICAST_HOPS, 5)

for i in range(1, 10):
    data = (b"Sending packet #%d\n" % (i))
    print(my_sock.sendto(data, (dst_ip, dst_port)))
    time.sleep(1)

my_sock.close()
