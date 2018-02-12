CC=bcm2708hardfp-gcc
CFLAGS=-O2 -static
CARD=22
NAME=lee-fabre
APP=helloworld

all: $(APP)
	scp -P 50$(CARD) $^ pi@132.227.102.36:lee-fabre

clean:
	rm $(APP)
