import string

lower_list = list(string.ascii_lowercase)
print(lower_list)

# By Index
for index in range(len(lower_list)):
    print("%c @ %d" % (lower_list[index], index))

# By Element
for element in lower_list:
    print("%c @ %d" % (element, lower_list.index(element)))