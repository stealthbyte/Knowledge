class A(object):
    def foo(self, m):
        self.x = m**2
        return self.x
        
class B(A):
    def foo(self, m):
        return 4 + super(B, self).foo(m)