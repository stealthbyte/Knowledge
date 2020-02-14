class A(object):
    def __init__(self):
        print('In Class A')

class B(object):
    def __init__(self):
        print('In Class B')

class C(A, B):
    pass

print('C.__mro__ - {}'.format(C.__mro__))
obj = C()


class A(object):
    def __init__(self):
        print('Entering Class A')
        super(A, self).__init__()
        print('Leaving Class A')

class B(object):
    def __init__(self):
        print('Entering Class B')
        super(B, self).__init__()
        print('Leaving Class B')

class C(A, B):
    def __init__(self):
        print('Entering Class C')
        super(C, self).__init__()
        print('Leaving Class C')

print('C.__mro__ - {}'.format(C.__mro__))
obj = C()