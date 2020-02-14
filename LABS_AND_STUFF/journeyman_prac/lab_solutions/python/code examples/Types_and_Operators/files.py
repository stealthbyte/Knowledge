fh = open("example.txt","rb")   # Open file "example.txt" for read, binary
if not fh.closed:               # Determine if fh is closed and if not...
    print(fh.fileno())           # ask for the fileno

data = fh.read()                # Returns entire contents of fh as a string
fh.seek(0)                      # Set file index to 0
print(fh.readline())             # Reads a line from the file
print(fh.readline(5))            # Read at most 5 bytes from the current
print(fh.readline())             # Read to the next line - returns the
print(fh.mode)                   # The I/O mode for the file
print(fh.name)                   # The Name of the file
fh.seek(0)                      # Seek to the 0th position
for count in range(0, 100):    # iterate from 0 to 99
    line = fh.readline()            # get the next line and
    print(line)                  # print it
print(fh.tell())                 # Tell us our offset
fh.seek(8*25)                   # Seek to the 200th position
try:
    fh.truncate()               # and TRUNCATE!!!
except Exception as e:
    print(e)
fh.close()