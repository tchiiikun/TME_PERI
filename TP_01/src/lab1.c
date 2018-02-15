#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "gpio.h"
#include "gpio_setup.h"
#include "gpio_value.h"

#define GPIO_LED0   4

int main(int argc, char** argv){

  uint32_t volatile * gpio_4 = (uint32_t volatile *)4;
  int period, half_period;

  if( setup_gpio_mmap(&gpio_4) == -1 ){
      perror("Error: gpio_setup\n");
      exit(1);
    }
    period = 1000; /* default = 1Hz */
    if ( argc > 1 ) {
      period = atoi ( argv[1] );
    }
    half_period = period / 2;
    gpio_setup_io(GPIO_LED0, 0, gpio_4);
    printf ( "-- info: start blinking.\n" );


    while (1) {
        gpio_update(GPIO_LED0, 1, gpio_4); // SET
        delay(half_period);
        gpio_update(GPIO_LED0, 0, gpio_4); // SET
        delay(half_period);
      }

       gpio_teardown();
       return 0;
}
