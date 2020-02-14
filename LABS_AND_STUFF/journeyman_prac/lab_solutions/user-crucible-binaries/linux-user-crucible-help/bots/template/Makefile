CC=gcc
CFLAGS=-Wall -Werror -Wextra -fPIC

BOT:= ircbot_template

all: $(BOT)

$(BOT): $(BOT).c
	$(CC) $(CFLAGS) -L../lib -I../lib/include -o $@ $^ -l:libircclient.a 

clean:
	rm -f $(BOT)
