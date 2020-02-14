def adder(a, b):
    return a + b
def multer(a, b):
    return a * b
def caller(f, a, b):
    return f(a, b)

print(caller(adder, 3, 4))
print(caller(multer, 3, 4))

print(caller(lambda x, y : x + y, 3, 4))
print(caller(lambda x, y : x * y, 3, 4))
f = lambda x : x ** 2
print(f(5))
print((lambda x, y, z : x + y * z)(4, 5, 6))