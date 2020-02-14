def f(a, b=1, c=1):
    return a * b * c

print(f(2))
print(f(2, 3))
print(f(2, 3, 5))
print(f(2, b=9))
print(f(a=2, c=4))

# *args
def pos(a, b, c):
    return [a, b, c]
def pos2(a, b, *args):
    print('a = %s (type = %s)' % (a, type(a)))
    print('b = %s (type = %s)' % (b, type(b)))
    print('args = %s (type = %s)' % (args, type(args)))
    return args + (a, b)

print(pos(1,2,3))
print(pos2(*[1,2,3]))
print(pos(*'ABC'))
print(pos2(*list(range(5))))


# **kwargs
def kw(a, b, **kwargs):
    print('a = %s (type = %s)' % (a, type(a)))
    print('b = %s (type = %s)' % (b, type(b)))
    print('kwargs = %s (type = %s)' % (kwargs, type(kwargs)))
    if kwargs.get('add_ab'):
        kwargs.update(a=a, b=b)
    return kwargs

print(kw(1, 2))
print(kw(1, 2, add_ab=True))
print(kw(**dict(add_ab=True, b='bbbb', a='aaaa', anotherthing='blah')))


def args_kwargs_test(*args, **kwargs):
    for arg in args:
        print("arg = {0}".format(arg))
    if "foo" in kwargs:
        print("foo's value = %s" % kwargs["foo"])
        
args_kwargs_test(1, 2, 3, 4, 5, 6, foo=5, bar=6)
args_kwargs_test(*[1, 2, 3, 4], **{"foo" : 5})