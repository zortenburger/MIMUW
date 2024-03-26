CC = gcc
CFLAGS = -g -Wall -Wextra -O2
LIBS = -lpthread

all: sikradio-sender sikradio-receiver

sikradio-sender: sikradio-sender.c err.h common.h
	$(CC) $(CFLAGS) -o sikradio-sender sikradio-sender.c

sikradio-receiver: sikradio-receiver.c err.h common.h buffer.h
	$(CC) $(CFLAGS) -o sikradio-receiver sikradio-receiver.c $(LIBS)

clean:
	rm -f sikradio-sender sikradio-receiver