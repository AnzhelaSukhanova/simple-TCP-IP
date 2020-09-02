CC=g++
CFLAGS=-c -Wall

all: main

del:
	@sudo bash tap.sh 1
	
arp: 
	@sudo bash tap.sh
	@sudo bash arp.sh
	
icmp:
	@sudo bash tap.sh
	@sudo bash icmp.sh

main: main.o dev.o frame.o proto.o tools.o
	$(CC) main.o dev.o frame.o proto.o tools.o -o main

dev.o: dev.c dev.h
	$(CC) $(CFLAGS) dev.c	
	
frame.o: frame.c frame.h
	$(CC) $(CFLAGS) frame.c
	
proto.o: proto.c proto.h
	$(CC) $(CFLAGS) proto.c	
	
tools.o: tools.c tools.h
	$(CC) $(CFLAGS) tools.c	
	
main.o: main.c proto.h
	$(CC) $(CFLAGS) main.c		

clean:
	rm -rf *.o main
