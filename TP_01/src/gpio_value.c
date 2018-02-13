#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <gpio.h>

uint32_t volatile* gpio_base_p;

int gpio_read(int gpio, int * val, uint32_t volatile*  ptr){

      *val = GPIO_VALUE(ptr, gpio);
      return 0;
}

int gpio_update(int gpio, int val, uint32_t volatile*  ptr){

      val == 1 ? GPIO_SET (ptr, gpio) :  GPIO_CLR (ptr, gpio);
      return 0;
}
