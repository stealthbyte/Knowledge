import ctypes

k32 = ctypes.WinDLL('kernel32.dll')
DeleteFile = k32.DeleteFileA
DeleteFile.argtypes = [ctypes.c_char_p]
DeleteFile.restype = ctypes.c_bool
buff = ctypes.c_char_p(b'C:\\deleteMe.txt')
ret_val = DeleteFile(buff)
print(ret_val)

print()
print()

#libc = ctypes.CDLL('libc.so.6')
#strchr = libc.strchr
#strchr.restype = ctypes.c_char_p
#strchr.argtypes = [ctypes.c_char_p, ctypes.c_char]
#print strchr('abcdef', 'd')
#try:
#    print strchr('abcdef', 'def') # we send string, not char
#except Exception as e:
#    print e

def valid_check(value):
    if value == 0:
        raise ctypes.WinError()
    return value

GetModuleHandle = ctypes.windll.kernel32.GetModuleHandleA
GetModuleHandle.restype = valid_check
try:
    GetModuleHandle('non-existent module') # returns 0
except Exception as e:
    print(e)

print()
print()

class Rect(ctypes.Structure):
    _fields_ = [('top_left_x',        ctypes.c_int),
                ('top_left_y',        ctypes.c_int),
                ('bottom_right_x',    ctypes.c_int),
                ('bottom_right_y',    ctypes.c_int)]

rect = Rect(10, 30, 40, 20)
print((rect.top_left_x, rect.bottom_right_x))

print()
print()

my_str = ctypes.c_char_p(b'abc def ghi')
print(my_str.value)
print(my_str.value == my_str.value) # equality check
print(my_str.value is my_str.value) # identity check

print()
print()

integer = ctypes.c_int(42)
pInteger = ctypes.pointer(integer)
print(pInteger.contents)
print(pInteger.contents.value == pInteger.contents.value)
print(pInteger.contents == pInteger.contents)
print(pInteger.contents is pInteger.contents)

print()
print()

s = b'Hello world' # define initial string
c_s = ctypes.c_char_p(s)
c_s.value = b'New Example' # save a new string value to object
print(c_s)
print(s) # original string value is unchanged

print()
print()

buff = ctypes.create_string_buffer(b'Hello')
print(ctypes.sizeof(buff), repr(buff.raw))
print(buff.value) # access it as a null-terminated string
buff.value = b'Hi'
print(buff.value)
print(repr(buff.raw))

print()
print()

buff = ctypes.create_string_buffer(4)
print(repr(buff.raw))
buff.value = b'Hi'
print(repr(buff.raw))
print(buff.value)