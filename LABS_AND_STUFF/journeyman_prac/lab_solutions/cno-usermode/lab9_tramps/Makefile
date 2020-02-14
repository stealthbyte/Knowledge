CC = gcc

main: main.o hooking.o hooking.h
	$(CC) -o main hooking.o main.o

main.o: main_solution.c hooking.h
	$(CC) -c -o main.o $<

hooking.o: hooking_solution.c
	$(CC) -c -o hooking.o $^

clean:
	rm main.o hooking.o main
