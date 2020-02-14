class MyError(Exception):
    def __init__(self, msg):
        super(MyError, self).__init__(msg)
        self.msg = msg

    def __str__(self):
        return "Your error was: %s" % self.msg

    def __repr__(self):
        return "The repr of your error was: %s" % self.msg


def DoDangerousStuff(filename):
    if filename is None:
        raise MyError("Dude, you screwed up!")

try:
    DoDangerousStuff(None)
except MyError as e:
    print(e)
    print(repr(e))