CC = gcc

main: main.o hooking.o hooking.h
	$(CC) -o main hooking.o main.o -ludis86

main.o: main.c hooking.h
	$(CC) -c -o main.o main.c

hooking.o: hooking.c
	$(CC) -c -o hooking.o hooking.c

clean:
	rm main.o hooking.o main
