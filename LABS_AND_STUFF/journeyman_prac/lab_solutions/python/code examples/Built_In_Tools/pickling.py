import pickle

class FooClass(object):
    pass
    
foo = FooClass()
foo.a = 1
print(foo)
print(foo.a)

print()
print()

pickled_foo = pickle.dumps(foo)
print(pickled_foo)

print()
print()

new_foo = pickle.loads(pickled_foo)
print(new_foo)
print(new_foo.a)

print(foo == new_foo)