#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <gpio.h>

#define GPIO_LED0   4

int main(void){

  int period, half_period;

  if( setup_gpio_mmap() == -1 ){
      perror("Error: gpio_setup\n");
      exit(1);
    }
    period = 1000; /* default = 1Hz */
    if ( argc > 1 ) {
      period = atoi ( argv[1] );
    }
    half_period = period / 2;
    gpio_setup_io(GPIO_LED0, 1, GPIO_OUTPUT_PIN);
    printf ( "-- info: start blinking.\n" );


    while (1) {
        gpio_update(GPIO_LED0, 1); // SET
        delay(half_period * 1000);
        gpio_update(GPIO_LED0, 0); // RESET
        delay(half_period * 1000);
      }

       gpio_teardown();
       return 0;
}
