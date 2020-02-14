class ActpList(object):
    def __init__(self, *args):
        self.myList = []

        for a in args:
          self.myList.append(a)

    def __add__(self,other):
      sumList = []
      i = 0

      while i < len(self.myList):
        if isinstance(other,ActpList):
          tmp = self.myList[i] + other.myList[i]
          sumList.append(tmp)
        elif isinstance(other,int):
          tmp = self.myList[i] + other
          sumList.append(tmp)
        i = i + 1

      return sumList

    def __getitem__(self,i):
      return hex(self.myList[i])

    def __setitem__(self,i, val):
      self.myList[i] = val*4

    def __str__(self):
      return str(self.myList)

    def __repr__(self):
      sortedList = []
      for i in self.myList:
        sortedList.append(i)

      sortedList.sort()

      hexList = []
      for i in sortedList:
        hexList.append(hex(i))
      return repr(hexList)



myList = ActpList(15,2,3,8,7,4,6)
myOtherList = ActpList(15,2,3,8,7,4,6)

res = myList + myOtherList
print("The summation of my two lists " + str(res))
res = myList + 5
print("myList + 5 = " + str(res))
res = myList[0]
print("myList[0] = " + res)
myList[5] = 4
print("myList modified " + str(myList))
print("Repr of the modified myList " + repr(myList))
print("My unchanged list " + str(myList))
