CC=gcc
CFLAGS=-Wall -Werror -Wextra -fPIE -pie

all: bothost

bothost: bothost.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f bothost
