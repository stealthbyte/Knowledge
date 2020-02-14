CC=gcc
CFLAGS=-Wall -Werror -Wextra

obj-m += pgd_drv.o

all: pgd_drv shareit

pgd_drv: pgd_drv.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

shareit: share.c
	$(CC) $(CFLAGS) -o $@ $^ -lrt

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ./shareit
