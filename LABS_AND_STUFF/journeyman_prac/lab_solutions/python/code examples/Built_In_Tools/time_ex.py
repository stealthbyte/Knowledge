import datetime
import time

print(datetime.datetime.now())

time.sleep(5)

d = datetime.datetime.now()
print(d.strftime('%A, %B %d %Y at %H:%M:%S'))