CFLAGS = -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2 -g
CC = gcc

.PHONY: all clean

all: labyrinth

labyrinth: main.o input.o string.o errors.o vector.o bitset.o queue.o output.o bfs.o
	$(CC) $(CFLAGS) -o $@ $^

main.o:  main.c 
	$(CC) -c $< -o main.o

input.o:  input.c input.h
	$(CC) -c $< -o input.o

output.o:  output.c output.h
	$(CC) -c $< -o output.o

string.o:  string.c string.h 
	$(CC) -c $< -o string.o

errors.o:  errors.c errors.h 
	$(CC) -c $< -o errors.o

vector.o:  vector.c vector.h 
	$(CC) -c $< -o vector.o

bitset.o:  bitset.c bitset.h 
	$(CC) -c $< -o bitset.o

queue.o:  queue.c queue.h
	$(CC) -c $< -o queue.o
	
bfs.o:  bfs.c bfs.h
	$(CC) -c $< -o bfs.o

clean:
	-rm input.o 
	-rm output.o 
	-rm string.o 
	-rm errors.o 
	-rm vector.o 
	-rm bitset.o 
	-rm queue.o 
	-rm bfs.o
	-rm main.o
	-rm labyrinth