CC=gcc
CFLAGS=-Wall -Werror -Wextra

obj-m += whatdodrv.o

all: whatdodrv whatdousr

whatdodrv: whatdodrv.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

whatdousr: whatdousr.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ./whatdousr
