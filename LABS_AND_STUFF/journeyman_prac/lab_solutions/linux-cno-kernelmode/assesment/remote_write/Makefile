CC=gcc
CFLAGS=-Wall -Werror -Wextra -pie -fPIE

obj-m += rw_driver.o

all: rw_driver rw_user

rw_driver: rw_driver.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

rw_user: rw_user.c
	$(CC) $(CFLAGS) -o $@ $^
	strip $@

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ./rw_user
	rm -f rw_dev
