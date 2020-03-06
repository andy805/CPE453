CC=gcc
CFLAGS = -Wall -g -fpic


all: clean minls minget

minls: minls.c
	$(CC) $(CFLAGS) -o minls minls.c

minget: minget.c
	$(CC) $(CFLAGS) -o minget minget.c

clean:
	rm -f *.o
