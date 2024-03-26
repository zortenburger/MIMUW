CC = gcc
CFLAGS = -g -Wall -Wextra -O2
LIBS = -lpthread
ADDITIONAL_FLAGS = buffer.h common.h constants.h data.h err.h fifo.h missing.h radio.h sikradio-controler.h \
	sikradio-listener.h sikradio-lookuper.h sikradio-player.h sikradio-rexmiter.h sikradio-switch.h \
	sikradio-ui.h vector.h

all: sikradio-sender sikradio-receiver

sikradio-sender: sikradio-sender.c $(ADDITIONAL_FLAGS)
	$(CC) $(CFLAGS) -o sikradio-sender sikradio-sender.c $(LIBS)

sikradio-receiver: sikradio-receiver.c $(ADDITIONAL_FLAGS)
	$(CC) $(CFLAGS) -o sikradio-receiver sikradio-receiver.c $(LIBS)

clean:
	rm -f sikradio-sender sikradio-receiver