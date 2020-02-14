class Bar(object):
    def __init__(self):
        self._x = None

    def getx(self):
        print("Calling getx")
        return self._x

    def setx(self, value):
        print("Calling setx")
        self._x = value

    def delx(self):
        print("Calling delx")
        del self._x

    x = property(getx, setx, delx, "I'm the 'x' property.")