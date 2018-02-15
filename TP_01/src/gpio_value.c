#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include "gpio.h"
#include "gpio_value.h"


int gpio_read(int gpio, int * val, uint32_t volatile*  ptr){

      *val = GPIO_VALUE(ptr, gpio);
      return 0;
}

int gpio_update(int gpio, int val, uint32_t volatile*  ptr){

      if (val == 1) {
       	GPIO_SET (ptr, gpio);
       } else {
        GPIO_CLR (ptr, gpio);
       }
      return 0;
}
