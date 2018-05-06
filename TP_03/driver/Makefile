CARD_NUMB       = 22
ROUTER          = 132.227.102.36
LOGIN           = lee-fabre
LAB             = lab3

TESTER          = lcd_uer_driver_LF
MODULE          = lcd_user_driver

CROSSDIR        = /users/enseig/franck/peri
KERNELDIR       = /dsk/l1/misc/linux-rpi-3.18.y
CROSS_COMPILE   = $(CROSSDIR)/arm-bcm2708hardfp-linux-gnueabi/bin/bcm2708hardfp-
        
obj-m           += $(MODULE).o
default:;       make -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules
clean:;         make -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) clean
upload:;        scp -P50$(CARD_NUMB) $(MODULE).ko pi@$(ROUTER):$(LOGIN)/$(LAB)
up:;        scp -P50$(CARD_NUMB) $(TESTER).c pi@$(ROUTER):$(LOGIN)/$(LAB)
