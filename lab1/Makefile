CC= gcc
CFLAGS = -Wall  -g -pedantic -ansi

all: pipeit clean

pipeit: pipeit.o
	$(CC) $(CFLAGS)  -o pipeit pipeit.c

clean:
	rm -f *.o
