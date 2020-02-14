def fib(top):
    a = 0
    b = 1
    while a <= top:
        yield a
        tmp = a + b
        a = b
        b = tmp

for i in fib(8):
    print(i)
    
gen = fib(3)
print(next(gen))
print(next(gen))
print(next(gen))
print(next(gen))
print(next(gen))
print(next(gen))