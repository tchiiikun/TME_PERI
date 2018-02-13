NAME=lee-fabre
CARD=22


CROSS_COMPILE = bcm2708hardfp-

CFLAGS=-Wall -Wfatal-errors -O2
LDFLAGS=-static -L. -lgpio

copy: *.x
	scp -P 50$(RPI) $^ pi@132.227.102.36:$(NAME)

libgpio.a: gpio_setup.o gpio_value.o
	$(CROSS_COMPILE)ar -rcs $@ $^

%.o: %.c
	$(CROSS_COMPILE)gcc -o $@ -c $(CFLAGS) $<

clean:
	rm -f *.o *.x *~ *.a
