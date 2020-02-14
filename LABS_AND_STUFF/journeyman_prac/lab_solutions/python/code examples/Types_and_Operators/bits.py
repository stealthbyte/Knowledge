# these are all hex numbers - NOT BINARY
print("0x%08x" % (0x00000010 | 0x00000001))    # or
print("0x%08x" % (0x00000010 & 0x00000001))    # and
print("0x%08x" % (0x00000010 ^ 0x00000001))    # xor
print("0x%08x" % (0x00000010 ^ 0x00000000))    # xor
print("0x%08x" % (0x00000001 << 4))            # shift left 4
print("0x%08x" % (0x00000001 << 8))            # shift left 8
print("0x%08x" % (0x00000001 << 12))           # shift left 12
print("0x%08x" % (0x00000001 << 16))           # shift left 16
print("0x%08x" % (0x00010000 >> 12))           # shift right 12

def bin(x):
    return "".join(x & (1 << i) and "1" or "0" for i in range(7,-1,-1))