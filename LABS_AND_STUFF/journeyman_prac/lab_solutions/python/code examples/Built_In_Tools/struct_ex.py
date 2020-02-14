import struct

a = 5
packed = struct.pack('B', a)
print(packed.decode())
print(repr(packed.decode()))

print()
print()

a = b'\x01\x02\x03\x04' # a struct we're going to unpack

little_endian = struct.unpack('<L', a) # little endian, unsigned long
print(little_endian)
big_endian = struct.unpack('>L', a) # big endian, unsigned long
print(big_endian)

print()
print()

a = bytearray([0x01, 0x02, 0x03, 0x04])
unpacked = struct.unpack('BBBB', a)
print(unpacked)
packed = struct.pack('>LBB', 5000, 4, 255)
print(packed)
print(repr(packed))
