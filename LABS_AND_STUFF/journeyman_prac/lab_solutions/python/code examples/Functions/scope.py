# global scope (module-level)
a = 1
b = 2

def f(b):
    # local scope (function level)
    a = 100
    b = 200
    return a, b

print(a, b)
print(f(3))
print(a, b)


def global_read():
    # a is global: never assignd to
    print(a)
def local_assign():
    # a is local: assigned to before read
    a = 2
    return a*a
def global_write_bad():
    # a is invalid: read from before assign
    a = a + 2
    return a

print(a)
print(global_read())
print(local_assign())
print(a)
try:
    print(global_write_bad())
except Exception as e:
    print(e)


a = 0
def global_inc_wrong():
    a += 1
def global_inc_right():
    global a
    a += 1

print(a)
try:
    global_inc_wrong()
except Exception as e:
    print(e)
global_inc_right()
print(a)
global_inc_right()
print(a)


a, b, c = 0, 1, []
def global_mod(n):
    c.append(n)
    return c
def multiple_globals():
    global a, b
    a, b = b, a + b
    return a, b
    
print(a, b, c)
print(global_mod(1))
print(global_mod(2))
print(multiple_globals())
print(multiple_globals())
print(multiple_globals())


def if_scoping(x):
    if x:
        if x:
            if x:
                a = 1
    return locals()
def loop_scoping():
    column = ['Something']
    for row in range(10):
        for column in range(row*100):
            pass
    return locals()
    
print(if_scoping(False))
print(if_scoping(True))
print(loop_scoping())