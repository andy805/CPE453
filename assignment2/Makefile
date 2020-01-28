CC=gcc
CFLAGS= -Wall -g -fpic
AR= ar r

all:  clean libmylwp.a intel-all

intel-all:   liblwp.so

liblwp.so: lib64 lwp64.o
	$(CC) $(CFLAGS) -shared -o $@ lwp64.o magic64.o

libmylwp.a:  lwp.o
	$(AR) libmylwp.a lwp.o 

prog: test_lib.o libmylwp.a magic64.o
	$(CC) $(CFLAGS) -o prog test_lib.o magic64.o libmylwp.a

test_lib.o: test_lib.c
	$(CC) $(CFLAGS) -c -o test_lib.o test_lib.c

lwp.o: lwp.c lwp.h magic64.o
	$(CC) $(CFLAGS) -c -o lwp.o lwp.c 

lwp64.o : lwp.c lwp.h
	$(CC) $(CFLAGS) -m64 -c -o lwp64.o lwp.c 

magic64.o: magic64.S
	$(CC) $(CFLAGS) -o magic64.o -c magic64.S

lib64:
	mkdir lib64

clean:
	rm -f *.o core.*

