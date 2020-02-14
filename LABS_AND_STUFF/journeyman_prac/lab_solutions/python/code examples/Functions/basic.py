def square(a):
    return a*a

def wacky(a,b):
    if a < b:
        return '%s[%r]' % (a, b)

print(square(2))
print(square(5))
print(wacky(1,2))
print(wacky(2,1))
print(wacky(2,1) == None)
print(wacky("a", "b"))

def tuple_1(a, b, c):
    return a, b, c

def tuple_2(a, b, c):
    return (a, b, c)

def list_1(a, b, c):
    return [a, b, c]

print(tuple_1(1,2,3))
print(tuple_2(1,2,3))
a,b,c = (1,2,3)
print((b,c,a))
a,b,c = list_1(1,2,3)
print((b,c,a))
try:
    a,b = tuple_1(1,2,3)
except Exception as e:
    print(e)
try:
    a,b,c,d = tuple_1(1,2,3)
except Exception as e:
    print(e)