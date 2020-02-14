CC=gcc
CFLAGS=-Wall -Werror -Wextra

obj-m += shuf_driver_2.o

all: shuf_driver_2 shuf_user_2

shuf_driver_2: shuf_driver_2.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

shuf_user_2: shuf_user_2.c
	$(CC) $(CFLAGS) -o $@ $^
	strip $@

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ./shuf_user_2
	rm -f shuf_dev_2
