CC=gcc
CFLAGS= -Wall -g -fpic
AR= ar r

all:  clean libmylwp.a intel-all prog

intel-all:   liblwp.so

liblwp.so: lib64 lwp64.o magic64.o
	$(CC) $(CFLAGS) -shared -o $@ lwp64.o magic64.o

libmylwp.a:  lwp.o magic64.o rr.o rr2.o
	$(AR) libmylwp.a lwp.o magic64.o rr.o rr2.o


prog: main.o  libmylwp.a magic64.o
	$(CC) $(CFLAGS) -o prog main.o magic64.o libmylwp.a

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

lwp.o: lwp.c lwp.h magic64.o
	$(CC) $(CFLAGS) -c -o lwp.o lwp.c magic64.o

lwp64.o : lwp.c lwp.h
	$(CC) $(CFLAGS) -m64 -c -o lwp64.o lwp.c 

magic64.o: magic64.S
	$(CC) $(CFLAGS) -o magic64.o -c magic64.S

rr.o: rr.c rr.h
	$(CC) $(CFLAGS) -o rr.o -c rr.c

rr2.o: rr2.c rr.h
	$(CC) $(CFLAGS) -o rr2.o -c rr2.c

lib64:
	mkdir lib64

clean:
	rm -f *.o core.*

