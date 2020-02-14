# global scope (module-level)
a = 1
b = 2

def f(b):
    # local scope (function level)
    a = 100
    b = 200
    return a, b

print(f(1))

a = 1
def global_read():
    # a is global: never assigned to
    print(a)

global_read()

def local_assign():
    # a is local: assigned to before read
    a = 2
    return a*a

print(local_assign())

def global_write_bad():
    # a is invalid: read from before assign
    try:
        a = a + 2
    except UnboundLocalError:
        return -1
    return a

print(global_write_bad())

a = 0
def global_inc_wrong():
    try:
        a += 1
    except UnboundLocalError:
        print("global_inc_wrong failed")
    

global_inc_wrong()

def global_inc_right():
    global a
    a += 1

global_inc_right()
print(a)

a, b, c = 0, 1, []

def global_mod(n):
    c.append(n)
    return c

print(global_mod(3))

def multiple_globals():
    global a, b
    a, b = b, a + b
    return a, b

def outer_f():
    a = 1
    b = 2
    def inner_f():
        a = 10
        c = 20
        def inner_f2():
            a = 100
            d = 200
            return locals()
        def inner_f3():
            res = locals()
            (a,b,c)
            return res
        return inner_f2(), inner_f3()
    return inner_f()

def outer_f2():
    a = 1
    def inner_f():
        try:
            a = a + 1
            return a
        except UnboundLocalError:
            import sys
            return sys.exc_info()[1]
    return inner_f()


def if_scoping(x):
    if x:
        if x:
            if x:
                a = 1
    return locals()

def loop_scoping():
    column = ['Something'] # overwritten
    for row in range(10):
        for column in range(row*100):
            pass # do something
    return locals()

    
