import os.path
import ctypes
import pdb


def valid_check(result):
    if result != 0:
        raise ctypes.WinError(result)

    return result


# Load DLL
directory = os.path.dirname(os.path.abspath(__file__))
dll = os.path.join(directory, 'vc6sneaky.dll')
sneaky = ctypes.windll.LoadLibrary(dll)


# encrypt1
msg1 = "Bruce Willis is a ghost!"
input1 = ctypes.create_string_buffer(bytes(msg1, "utf-8"))

size1 = ctypes.sizeof(input1)
output1 = ctypes.create_string_buffer(size1)

encrypt1 = sneaky.encrypt1
encrypt1.restype = valid_check
encrypt1.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_uint32]
encrypt1(input1, output1, size1)

print('Encrypted version of "%s" is %r' % (msg1, output1.raw))

output2 = ctypes.create_string_buffer(size1)
sneaky.encrypt1(output1, output2, size1)
print('Encrypted version of %r is "%s"' % (output1.raw, output2.value.decode()))
print()


# encrypt2
class SneakyStruct(ctypes.Structure):
    _fields_ = [
        ("size",    ctypes.c_uint32),
        ("input",   ctypes.c_char_p),
        ("output",  ctypes.c_char_p)
    ]


encrypt2 = sneaky.encrypt2
encrypt2.restype = valid_check
encrypt2.argtypes = [ctypes.POINTER(SneakyStruct)]

msg2 = "Snape kills Dumbledore!"
input3 = ctypes.create_string_buffer(bytes(msg2, "utf-8"))
size2 = ctypes.sizeof(input3)
output3 = ctypes.create_string_buffer(size2)

struct1 = SneakyStruct(size2,
                       ctypes.cast(input3, ctypes.c_char_p),
                       ctypes.cast(output3, ctypes.c_char_p))

encrypt2(ctypes.pointer(struct1))

print('Encrypted version of "%s" is %r' % (msg2, output3.raw))

input4 = output3
output4 = ctypes.create_string_buffer(size2)

struct2 = SneakyStruct(size2,
                       ctypes.cast(input4, ctypes.c_char_p),
                       ctypes.cast(output4, ctypes.c_char_p))
encrypt2(ctypes.pointer(struct2))

print('Encrypted version of %r is "%s"' % (input4.raw, struct2.output.decode()))
print()

# Bonus
sneakyfile = os.path.join(directory, 'sneakyfile.bin')
data = open(sneakyfile, 'rb').read()

input5 = ctypes.create_string_buffer(data)
size3 = ctypes.sizeof(input5)
output5 = ctypes.create_string_buffer(size3)

encrypt1(input5, output5, size3)

out_file = os.path.join(directory, 'out.txt')
with open(out_file, 'wb') as fh:
    fh.write(output5.raw)


# Bonus Bonus
input6 = ctypes.create_string_buffer(255)
size4 = ctypes.sizeof(input6)
output6 = ctypes.create_string_buffer(size4)

encrypt1(input6, output6, size4)
key = output6.value[:20]
print('Encryption key = "%s"' % key.decode())


# Bonus Bonus Bonus
def decrypt(my_input):
    out_str = ''

    for index, char in enumerate(my_input):
        out_str += chr(ord(char) ^ ord(key[index % len(key)]))

    return out_str
