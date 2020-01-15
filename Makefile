CC=gcc
AR=ar r
CFLAGS= -Wall -g -pedantic

all: libmymalloc.a prog clean

libmymalloc.a: lib my_malloc.o
	$(AR) libmymalloc.a my_malloc.o 

prog: test_lib.o libmymalloc.a
	$(CC) $(CFLAGS) -o prog test_lib.o libmymalloc.a

test_lib.o: test_lib.c
	$(CC) $(CFLAGS) -c -o test_lib.o test_lib.c


my_malloc.o : my_malloc.c header_my_malloc.h
	$(CC) $(CFLAGS) -c -o my_malloc.o my_malloc.c 

lib:
	mkdir lib

clean:
	rm -f *.o
