#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <gpio_macros.h>

uint32_t volatile* gpio_base_p;


/**
  * Lecture d'une broche
  * gpio : un numéro de broche légal
  * *val : pointeur vers un buffer qui contiendra la valeur lue
  * Rend 0 si pas d'erreur; -1 en cas d'erreur
  **/
int gpio_read(int gpio, int * val, uint32_t volatile*  ptr){

      *val = GPIO_VALUE(ptr, gpio);
      return 0;
}


/** Ecriture d'une broche Ou Clear de la broche
  * gpio : un numéro de broche légal
  * val : valeur à écrire
  * Rend 0 si pas d'erreur; -1 en cas d'erreur
  **/
int gpio_update(int gpio, int val, uint32_t volatile*  ptr){

      val == 1 ? GPIO_SET (ptr, gpio) :  PIO_CLR (ptr, gpio);
      return 0;
}
