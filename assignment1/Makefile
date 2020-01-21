CC=gcc
CFLAGS= -Wall -g -fpic
AR= ar r

all:  clean libmymalloc.a intel-all

intel-all:  lib/libmalloc.so lib64/libmalloc.so

lib/libmalloc.so: lib malloc32.o
	$(CC) $(CFLAGS) -m32 -shared -o $@ malloc32.o

lib64/libmalloc.so: lib64 malloc64.o
	$(CC) $(CFLAGS) -shared -o $@ malloc64.o

libmymalloc.a:  my_malloc.o
	$(AR) libmymalloc.a my_malloc.o 

prog: test_lib.o libmymalloc.a
	$(CC) $(CFLAGS) -o prog test_lib.o libmymalloc.a

test_lib.o: test_lib.c
	$(CC) $(CFLAGS) -c -o test_lib.o test_lib.c

my_malloc.o: my_malloc.c header_my_malloc.h
	$(CC) $(CFLAGS) -c -o my_malloc.o my_malloc.c

malloc32.o : my_malloc.c header_my_malloc.h  
	$(CC) $(CFLAGS) -m32 -c -o malloc32.o my_malloc.c 

malloc64.o : my_malloc.c header_my_malloc.h 
	$(CC) $(CFLAGS) -m64 -c -o malloc64.o my_malloc.c 

lib:
	mkdir lib

lib64:
	mkdir lib64

clean:
	rm -f *.o core.*
