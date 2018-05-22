RPI?=22
SRC=src
APP=NRF24L01_base
DST=lee-fabre/lab7

CROSSDIR        = /users/enseig/franck/peri
CROSS_COMPILE   = $(CROSSDIR)/arm-bcm2708hardfp-linux-gnueabi/bin/bcm2708hardfp-

INC=$(HOME)/rf24/include
LIB=$(HOME)/rf24/lib
CFLAGS=-Wall -Wfatal-errors -O2 -I$(INC)
LDFLAGS=-L$(LIB) -lrf24

all:  $(APP).x

$(APP).x: $(APP).c
	$(CROSS_COMPILE)g++ -o $@ -I$(INC) $<  -O2 $(LDFLAGS)

upload: 
	scp -P50$(RPI) $(APP).x pi@peri:$(DST)
	
up: 
	scp -P50$(RPI) $(APP).c pi@peri:$(DST)

upmake: 
	scp -P50$(RPI) Makefile pi@peri:$(DST)


clean:
	rm -f *.o *.x *~
