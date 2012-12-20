CC 	= gcc
CFLAGS 	= -g
LDFLAGS	=

all: avoidance


avoidance: avoidance.o IAvoid.o
	$(CC) $(LDFLAGS) -o avoidance avoidance.o IAvoid.o

avoidance.o: main.c
	$(CC) $(CFLAGS) -c  main.c -o avoidance.o

IAvoid.o: IAvoid.c IAvoid.h
	$(CC) $(CFLAGS) -c  IAvoid.c -o IAvoid.o

clean:
	rm *~; rm *.o; rm avoidance