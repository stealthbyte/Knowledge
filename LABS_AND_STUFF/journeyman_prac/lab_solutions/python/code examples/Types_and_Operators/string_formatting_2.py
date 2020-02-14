x = 'foo'
y = 'bar'
'%s = %s' % (x, y)
print('%s = %s' % (x, y))
x = 1234
print('integers: ...%d...%-6d...%06d' % (x, x, x))
x = 1.23456789999
print('%e ... %f ... %g' % (x, x, x))
print('%-6.2f ... %05.2f ... %+06.1f' % (x, x, x))
x = "so this is %s"
print(x % 'fun')
y = '%03d'
print(y % 4)
z = 4
print(x % (y % z))
try:
    print('% ' % x)
except Exception as e:
    print(e)
try:
    print('% \n' % x)
except Exception as e:
    print(e)
