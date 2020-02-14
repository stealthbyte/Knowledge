# Concatenation
x = "asdf"
y = x + " space " + "words"
print(y)

# Repetition
z = x * 3
print(z)

# Exploding
l = list(x)
print(l)

# Imploding
new_x = ''.join(l)
print(new_x)

x = "so get this"
print(x.split("get"))
print(x)
print(x.replace("get","forget"))
print(x)
where = x.find("get th")
try:
    x[where] = "what is "
except Exception as e:
    print(e)
print(x)
l = len("get th")
x = x[:where] + "what " + x[where + l:]
print(x)