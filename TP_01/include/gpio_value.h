#ifndef _GPIO_VALUE_H_
#define _GPIO_VALUE_H_

#include <stdint.h>

/**
  * Lecture d'une broche
  * gpio : un numéro de broche légal
  * *val : pointeur vers un buffer qui contiendra la valeur lue
  * Rend 0 si pas d'erreur; -1 en cas d'erreur
  **/
int gpio_read(int gpio, int * val, uint32_t volatile*  ptr);


/** Ecriture d'une broche Ou Clear de la broche
  * gpio : un numéro de broche légal
  * val : valeur à écrire
  * Rend 0 si pas d'erreur; -1 en cas d'erreur
  **/
int gpio_update(int gpio, int val, uint32_t volatile*  ptr);

#endif
