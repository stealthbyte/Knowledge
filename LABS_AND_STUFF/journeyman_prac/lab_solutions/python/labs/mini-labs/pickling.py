import pickle


# dump/load
try:
    print(1/0)
except Exception as exc:
    fh = open('exception.txt', 'wb')
    pickle.dump(exc, fh)
    fh.close()
    
fh = open('exception.txt', 'rb')
loaded_exc = pickle.load(fh)
fh.close()

print('dump/load example:')
print(loaded_exc)
print()


# dumps/loads
try:
    print(x)
except Exception as exc:
    dumps_string = pickle.dumps(exc)
    
loads_string = pickle.loads(dumps_string)
print('dumps/loads example:')
print(loads_string)
