CC=g++
CFLAGS=-c

all: pfs
pfs: pfs.o
	$(CC) pfs.o -o pfs
pfs.o: pfs.cpp
	$(CC) $(CFLAGS) pfs.cpp

clean:
	rm -rf *o pfs