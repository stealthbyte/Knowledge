d = {'one': 1, 'two': 2, 'three': 3}
keys = d.keys()
values = d.values()
for k, v in d.items():
    print(k, v)
print('keys: {}'.format(list(keys)))
print('values: {}'.format(list(values)))
del d['one']
print(list(keys))

dict_x = {"zero": "a", "one": "s", "two": "d", "three": "f"}
print(dict_x)
dict_y = dict(zero="a", one="s", two="d", three="f")
print(dict_y)

dict_x = {}                     # Create an empty dictionary
dict_x["one"] = 1               # Add an item to a dictionary
print(dict_x)
dict_x = {"one" : 1}            # Create a dictionary with an item
print(dict_x)
try:
    dict_y["two"] = 2           # Attempt to add an item to an undeclared dictionary
except Exception as e:
    print(e)
dict_y = {}                     # Create another empty dictionary
dict_y["two"] = 2               # Set item key "two" equal to 2
dict_y["two"] = 3               # Set item key "two" equal to 3
dict_y["three"] = 3             # Add a new item "three" = 3
print(dict_y)
try:
    dict_y["one"]
except Exception as e:
    print(e)
print(dict_y)
print(dict_y.items())            # Get all items - returns a list of tuples
print(dict_y.keys())             # Get all keys - returns a list
print(dict_y.values())           # Get all values - returns a list
print("two" in dict_y)     # Determine if the dictionary has a specific key
print(dict_y.popitem())          # Remove and return an arbitrary item
print(dict_y)
print(dict_y.pop("three", 0))    # Remove and return the value associated with key
print(dict_y)
print(dict_y.pop("three", 0))    # Remove and return the value associated
                                # with the key - default to 0 if key not found
print(dict_y)
print(dict_x)
dict_y.clear()                  # Clear all items from a dictionary
dict_y["four"] = 4              # Add an item to a dictionary
print(dict_y)
dict_y.update(dict_x)           # Add all unique items from dict_x to dict_y
                                # Overwrite all items in dict_y who share a key in dict_x
print(dict_y)
print(dict_x)