list_x = []                 # create an empty list
list_y = [1, 2, 3]          # create a non-emtpy list
print(list_y)
list_x.append("asdf")       # add (to the end) "asdf"
print(list_x)
list_x.insert(0, ";lkj")    # insert at index 0 ";lkj"
print(list_x)
list_x.append(list_y)       # create a nested list
print(list_x)
list_z = [4,5,6]            # create a new list
print(list_y)
list_y.extend(list_z)       # add to the end of list_y all elements of list_z
print(list_y)
print(list_x)