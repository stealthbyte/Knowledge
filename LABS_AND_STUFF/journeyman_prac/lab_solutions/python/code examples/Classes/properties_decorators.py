# Instead of using property() directly as in the slides you can use decorators to accomplish the same thing
# property() returns a specific object
#
#>>> property()
# <property object at 0x02A75480>
# 
#>>> x = property()
#>>> x.fget
#>>> x.fget is None
#True
#>>> x.fset is None
#True
#>>> x.fdel is None
#True
#>>> 
#>>> def myGet(self): print "Hello from getter!"

#>>> x = property(myGet)
#>>> x.fget is myGet
#True
#>>> x.fset is None
#True
#>>> 
#
# So Bar.x gets replaced with a property value 
# when we talked about "Customization using Special Methods"
# we mentioned that certain special functions allow you to define
# special behavior for your class to emulate primitive access (e.g. +, =, etc)
# one of these functions is __get__() which defines special behavior for when
# a caller uses the '=' operand to retrieve an attribute value (e.g. y = Bar.x)
# There are also __set__() and __delete__() which work the same way
#
# So at a high level:
# 1)
#     @property
#     def x(self):
#     [...snip...]
# 2) Internally the python interpreter does what we talked about in the slides:
#     x = property(...)
# 3) x is now a property object associated with our Bar class
# 4) The property object has fget(), fset() and fdel() which you can define when you
#    construct the object
# 5) The Property class has __get__() et al defined internally, so when someone does
#    y = Bar.x it calls Property's __get__() which will call the fget() that you set,
#    either via the property() call or with an @property tag
class Bar(object):
    def __init__(self):
        self._x = None

    # this decorator tells the interpreter to build a property object with
    # the fget set to this
    @property 
    def x(self):
        """Dealing with 'x' property"""
        return self._x

    # this decorator tells the interpreter to modify the existing property object
    # to add a setter function
    @x.setter
    def x(self,value):
        print("Calling x setter")
        self._x = value


    @x.deleter
    def x(self):
        del self._x
