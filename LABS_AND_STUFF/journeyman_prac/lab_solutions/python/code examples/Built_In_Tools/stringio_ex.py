import io

msg = 'This is a test'
f = io.StringIO(msg)
print(f.read())

f = io.StringIO()
f.write('This is ')
f.write('a test')
print(f.getvalue())