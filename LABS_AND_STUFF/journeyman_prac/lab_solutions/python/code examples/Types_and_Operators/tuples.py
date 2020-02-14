tuple_x = "a","b","c","d"       # Create a basic tuple
print(tuple_x)
for char in tuple_x:            # Iterate through the items
    print(char, ' ', end='')    # and print them on the same line (and without a newline at the end)
print("".join(tuple_x))         # Join the items into a string
char_a = "a"
if char_a in tuple_x:           # Check for membership "in" tuple_x
    print("in there")
print(tuple_x[0:3])             # Slicing
print(tuple_x * 3)              # Repetition
print("".join(tuple_x * 3))     # Repetition and string joining
x , y = tuple_x[0:2]            # Only grab the first 2 items
print("x = %c; y = %c" % (x, y))
