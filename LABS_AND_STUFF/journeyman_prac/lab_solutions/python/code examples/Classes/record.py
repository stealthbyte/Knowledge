class Record(object):
    e = 2
    
    def __init__(self, d):
        self.d = d
        
    def __str__(self):
        return self.d
        
    def foo(self, m):
        self.d *= m
        return self.d