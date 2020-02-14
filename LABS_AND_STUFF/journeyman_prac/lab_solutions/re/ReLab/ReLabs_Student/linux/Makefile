# Dependencies are make, nasm, g++, libstdc++.i686, glibc-devel.i686
CC=g++
CFLAGS=-g -Wall -masm=intel -m32

EXEC:= ReLabs
SRCS:= $(wildcard *.cpp)
HDRS:= $(wildcard *.h)
OBJS:= $(SRCS:.cpp=.o)
LIBS:= -ldl

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS)
	$(CC) -m32 -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm $(EXEC) $(OBJS)
