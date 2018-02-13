#ifndef _GPIO_SETUP_H_
#define _GPIO_SETUP_H_

/**
  * L'adresse de base des registres GPIO dans l'espace d'adressage physique est supposée connue,
  * mais nous verrons que l'on peut savoir où se trouve les GPIO en lisant des variables exposées par le noyau.
  * Rend 0 si pas d'erreur; -1 en cas d'erreur.
  **/
int setup_gpio_mmap(uint32_t volatile ** ptr);

/** Setup des broches en INPUT ou OUTPUT
  * gpio : un numéro de broche légal
  * direction : 0 = INPUT, 1 = OUTPUT
  * Rend 0 si pas d'erreur; -1 en cas d'erreur.
**/
int gpio_setup_io(int gpio, int direction, uint32_t volatile*  ptr);
/**
  * unmap
  **/
void teardown_gpio_mmap(void * ptr){;

/**
  * Fait un sleep
  **/
void delay(unsigned int milisec);

#endif
