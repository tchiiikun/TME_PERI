RPI?=22
SRC=src
APP=NRF24L01_base
DST=lee-fabre/lab7

CROSSDIR        = /users/enseig/franck/peri

INC=$(HOME)/RF24-master/include 
INCSQ=/usr/include
LIB=$(HOME)/RF24-master/lib  
LIBSQ=/usr/lib
CFLAGS=-Wall -Wfatal-errors -O2 -I$(INC) -I$(INCSQ)
LDFLAGS=-L$(LIB) -L$(LIBSQ) -lrf24

all:  $(APP).x

$(APP).x: $(APP).cpp 
	g++ -o $@ -I$(INC) -I$(INCSQ) $<  -O2 $(LDFLAGS) -lsqlite3

clean:
	rm -f *.o *.x *~
	rm *.db
