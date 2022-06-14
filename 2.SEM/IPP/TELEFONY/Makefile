CC = gcc

CFLAGS   = -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2 -O0 -ggdb3

MY_FILES = phone_forward.o phone_forward_example.o trie.o trieRev.o string.o phone_numbers.o

all: phone clean

phone: $(MY_FILES)
	$(CC) $(CFLAGS) -o phone $(MY_FILES)

phone_forward_example.o: phone_forward_example.c

phone_forward.o: phone_forward.c

trie.o: trie.c

trieRev.o: trieRev.c

string.o: string.c

phone_numbers.o: phone_numbers.c

clean:
	rm phone_forward.o phone_forward_example.o  trie.o trieRev.o string.o phone_numbers.o 