CC=gcc
CFLAGS=-Wall -Werror -Wextra

all: exploit_vlnmodule

exploit_vlnmodule: exploit_vlnmodule.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f ./exploit_vlnmodule
