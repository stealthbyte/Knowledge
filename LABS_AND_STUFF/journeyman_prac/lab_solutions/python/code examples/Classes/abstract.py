import abc

class MyAbstractClass(object):
    __metaclass__ = abc.ABCMeta

    @abc.abstractmethod
    def my_func(self):
        pass

    @abc.abstractproperty
    def var(self):
        pass


class MyClass(MyAbstractClass):
    def __init__(self, arg):
        self._arg = arg

    def my_func(self):
        return 'foo'

    @property
    def var(self):
        return self._arg

