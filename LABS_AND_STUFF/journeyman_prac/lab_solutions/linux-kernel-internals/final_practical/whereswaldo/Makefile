CC = gcc
CFLAGS= -O3 -Wall
 
EXEC:= waldo
SRCS:= $(wildcard *.c)
HDRS:= $(wildcard *.h)
OBJS:= $(SRCS:.c=.o)
LIBS:= 

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)
	strip $@

%.o: %.c $(HDRS)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(EXEC) $(OBJS)



