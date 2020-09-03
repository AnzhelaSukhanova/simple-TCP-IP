CC=gcc
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

objects = main.o dev.o proto.o tools.o

main: $(objects)
	$(CC) -o main $(objects)

dev.o: dev.h		
proto.o: proto.h frame.h dev.h tools.h	
tools.o: tools.h		
main.o: proto.h	

clean:
	rm -rf *.o main
