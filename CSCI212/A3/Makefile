SHELL=/bin/sh

CC=CC

INCLUDES=

LIBS=-lsocket -lnsl -lresolv

CFLAGS=-g $(INCLUDES)

.SUFFIXES:
.SUFFIXES: .cpp $(SUFFIXES)

.cpp.o:
	$(CC) $(CFLAGS) -c $<

.cc.o:
	$(CC) $(CFLAGS) -c $<

TARGETS=server client

# 'make all' will build both server and client targets.
all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

# ****
#
# This example is made up of three files, TCP_fns.cc, EchoServer.cc
# and EchoClient.cc. Yours will consist of different files. You should
# modify these dependencies appropriately.
#
# ****

wrappers.o: wrappers.h
EchoServer.o: wrappers.h
EchoClient.o: wrappers.h

# ***** NOTE *****
#
# For submission purposes, create a makefile like this one that
# has at least two targets, 'server' and 'client', so that a test
# compilation can be performed automatically.
#
# ***** NOTE *****

server: server.o wrappers.o
	$(CC) -o $@ server.o wrappers.o $(LIBS)

client: client.o wrappers.o
	$(CC) -o $@ client.o wrappers.o $(LIBS)


