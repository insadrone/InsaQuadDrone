CC=gcc
CFLAGS=-g -O2 -Wall
LDFLAGS=-O2

all: test

test: config_bluetooth_connexion.o
	$(CC) -o test $(LDFLAGS) config_bluetooth_connexion.o

config_bluetooth_connexion.o: config_bluetooth_connexion.c config_bluetooth_connexion.h
	$(CC) $(CFLAGS) -c config_bluetooth_connexion.c

clean:
	rm test *.o

