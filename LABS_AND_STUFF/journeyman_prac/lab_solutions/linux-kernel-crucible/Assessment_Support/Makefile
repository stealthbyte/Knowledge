CC=gcc
CFLAGS=-Wall -Werror -Wextra -s -g --strip-all

obj-m += vlnmodule.o

all: vlnmodule

vlnmodule: vlnmodule.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
