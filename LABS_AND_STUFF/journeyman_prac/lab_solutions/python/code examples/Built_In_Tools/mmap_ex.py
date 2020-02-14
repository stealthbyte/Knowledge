import mmap

f = open('foo.txt', 'r+b') # assumes there is a file named 'foo.txt' in the local directory
                            #Also assumes that file contents are 10 bytes
                            #File provided with contents "testing123"
mm = mmap.mmap(f.fileno(), 0) # entire file
print(mm.readline())
print(mm[:5])
mm[6:] = b'blah' # new content must have same length
mm.seek(0)
print(mm.readline())
mm.close()