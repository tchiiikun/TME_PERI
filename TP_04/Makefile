CARD_NUMB       = 23
ROUTER          = 132.227.102.36
LOGIN           = lee-fabre
LAB             = lab4

# TESTER          = lcd_suer_driver_LF
FILES 		= server-fake

CROSSDIR        = /users/enseig/franck/peri
KERNELDIR       = /dsk/l1/misc/linux-rpi-3.18.y
CROSS_COMPILE   = $(CROSSDIR)/arm-bcm2708hardfp-linux-gnueabi/bin/bcm2708hardfp-

# obj-m           += $(MODULE).o
# default:;       make -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules
# clean:;         make -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) clean
tarball:; 	tar -cvf $(FILES).tar $(FILES)
upload:;        scp -P50$(CARD_NUMB) $(FILES).tar pi@$(ROUTER):$(LOGIN)/$(LAB)
# up:;        scp -P50$(CARD_NUMB) $(TESTER).c pi@$(ROUTER):$(LOGIN)/$(LAB)

clean:
	rm *.tar
