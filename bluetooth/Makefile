CC=gcc
CFLAGS=-g -O2 -Wall
LDFLAGS=-O2

all: test

test: config_bluetooth_connexion.o Get_GPS_data_bluetooth.o gps.o udp_client.o
	$(CC) -o test $(LDFLAGS) config_bluetooth_connexion.o Get_GPS_data_bluetooth.o gps.o udp_client.o

gps.o: gps.c gps.h
	$(CC) $(CFLAGS) -c gps.c

udp_client.o: udp_client.c udp_client.h
	$(CC) $(CFLAGS) -c udp_client.c

Get_GPS_data_bluetooth.o: config_bluetooth_connexion.c config_bluetooth_connexion.h gps.h udp_client.h
	$(CC) $(CFLAGS) -c Get_GPS_data_bluetooth.c

config_bluetooth_connexion.o: config_bluetooth_connexion.c config_bluetooth_connexion.h
	$(CC) $(CFLAGS) -c config_bluetooth_connexion.c

clean:
	rm test *.o

