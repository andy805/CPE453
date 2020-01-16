CC=gcc
AR=ar r
CFLAGS= -Wall -g -pedantic

all: libmymalloc.a prog clean


libmymalloc.a:  my_malloc.o 
	$(AR) libmymalloc.a my_malloc.o 

prog: test_lib.o libmymalloc.a
	$(CC) $(CFLAGS) -o prog test_lib.o libmymalloc.a


test_lib.o: test_lib.c
	$(CC) $(CFLAGS) -c -o test_lib.o test_lib.c

my_malloc.o : my_malloc.c header_my_malloc.h helper.o helper.h
	$(CC) $(CFLAGS) -c -o my_malloc.o my_malloc.c helper.o

helper.o: helper.c helper.h
	$(CC) $(CFLAGS) -c -o helper.o helper.c

clean:
	rm -f *.o
