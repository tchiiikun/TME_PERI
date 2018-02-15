#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <time.h>
#include "gpio.h"
#include "gpio_setup.h"


static int mmap_fd;

/**
  * L'adresse de base des registres GPIO dans l'espace d'adressage physique est supposée connue,
  * mais nous verrons que l'on peut savoir où se trouve les GPIO en lisant des variables exposées par le noyau.
  * Rend 0 si pas d'erreur; -1 en cas d'erreur.
  **/
int setup_gpio_mmap(uint32_t volatile ** ptr){

    void * mmap_result;
    mmap_fd = open ("/dev/mem", O_RDWR | O_SYNC);

    if (mmap_fd < 0) {return -1;}

    mmap_result = mmap (
            NULL
            ,RPI_BLOCK_SIZE
            ,PROT_READ | PROT_WRITE
            ,MAP_SHARED
            ,mmap_fd
            ,BCM2835_GPIO_BASE );

    if (mmap_result == MAP_FAILED) {
        close (mmap_fd);
        return -1;
    }

    *ptr = (uint32_t volatile *)mmap_result;
    return 0;
}

/** Setup des broches en INPUT ou OUTPUT
  * gpio : un numéro de broche légal
  * direction : 0 = INPUT, 1 = OUTPUT
  * Rend 0 si pas d'erreur; -1 en cas d'erreur.
**/
int gpio_setup_io(int gpio, int direction, uint32_t volatile*  ptr) {

    if (direction == 1) {
    	 		GPIO_CONF_AS_INPUT (ptr, gpio);
    	 } else {
    	 	 	GPIO_CONF_AS_OUTPUT (ptr, gpio);
    	 }
    return 0;
}
/**
  * unmap
  **/
void teardown_gpio_mmap(void * ptr){
    munmap (ptr, RPI_BLOCK_SIZE);
}

/**
  * Fait un sleep
  **/
void delay(unsigned int milisec){
    struct timespec ts, dummy;
    ts.tv_sec  = ( time_t ) milisec / 1000;
    ts.tv_nsec = ( long ) ( milisec % 1000 ) * 1000000;
    nanosleep ( &ts, &dummy );
}
