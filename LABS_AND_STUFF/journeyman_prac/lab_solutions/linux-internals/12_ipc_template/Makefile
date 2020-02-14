CC = gcc
CFLAGS=-g -Wall
 
EXEC:= 12_ipc
SRCS:= $(wildcard *.c)
HDRS:= $(wildcard *.h)
OBJS:= $(SRCS:.c=.o)
LIBS:= -pthread

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS)
		$(CC) -o $@ $^ $(LIBS)

%.o: %.c $(HDRS)
		$(CC) -c $(CFLAGS) -o $@ $< $(LIBS)

clean:
		rm -f $(EXEC) $(OBJS)

