&nbsp;

# <center> TP1 : Outils de développement et GPIO </center>

&nbsp;

## 0. Plateforme de développement
&nbsp;

La Raspberry qui nous a été attribué est la Rasberry 22.

Pour se connecter à la Raspberry PI, il faut écrire : 

	$ ssh -p 5022 pi@132.227.102.36
	pi@132.227.102.36's password: raspberry 

Pour envoyer un fichier vers le dossier, il faut écrire : 
 
	$ scp -P 5022 file.x pi@132.227.102.36:lee-fabre
	pi@132.227.102.36's password: raspberry 

Pour éviter de mettre le mot de passe à chaque connexion, il faut :

	ssh-keygen -t rsa
	ssh-copy-id -i .ssh/id_rsa.pub "-p 5022 pi@132.227.102.36"
	ssh -p 5022 pi@132.227.102.36

&nbsp;

## 1. Prise en mains des outils de développement: Hello World !

&nbsp;

Pour pouvoir utiliser le compilateur, on doit le configurer dans le terminal et le vérifier :

	$ source /users/enseig/franck/peri/export_rpi_toolchain.sh
	$ which bcm2708hardfp-gcc

Avant toute chose, il faut écrire un programme C pour afficher **"Hello World !"** :

	#include <stdio.h>
	#include <stdlib.h>
	
	int main(void) {
		printf("Hello World !\n");
		return 0;
	}

et écrire un **Makefile** pour le compiler et l'envoyer sur la Rasbpberry : 

	NAME=lee-fabre
	CARD=22
	
	CROSS_COMPILE = bcm2708hardfp-
	
	CFLAGS=-Wall -Wfatal-errors -O2
	LDFLAGS=-static -L. -lgpio
	
	copy: *.x
		scp -P 50$(RPI) $^ pi@132.227.102.36:$(NAME)
	
	libgpio.a: gpio_setup.o gpio_value.o
		$(CROSS_COMPILE)ar -rcs $@ $^
	
	%.o: %.c
		$(CROSS_COMPILE)gcc -o $@ -c $(CFLAGS) $<
	
	clean:
		rm -f *.o *.x *~ *.a

&nbsp;

## 2. Contrôle de GPIO en sortie

&nbsp;

Pour pouvoir manipuler une pin du GPIO en mode "sortie" pour contrôler le clignotement d'une LED à une fréquence donnée. 

Pour réaliser ceci, on doit :

* allouer en mémoire à travers **mmap** pour obtenir le pointeur sur la zone mémoire attribué au GPIO 
* configurer la sortie pour connecter la LED à la zone mémoire du GPIO
* allumer la LED en alternant '1' et '0' d'une demi période
* désallouer la zone mémoire

On récupère le dossier qui contient les fichiers nécessaires : 

	cp -rp /users/enseig/franck/peri/lab1

On observe que la LED s'allume et d'éteigne de manière alterner.

> Code à compléter

&nbsp;

## 3. Contrôle de plusieurs GPIO en mode "sortie"

&nbsp;

Dans cette partie, on doit faire clignoté les deux LEDS : 

* LED0 connecté sur le GPIO4
* LED1 connecté sur le GPIO17

On a choisi d'alterner les deux LEDS, lorsqu'une des LEDS s'allume, l'autre LED s'éteigne.

> Code à compléter

&nbsp;

## 4. Lecture de la valeur d'une entrée GPIO

&nbsp;

Après avoir traiter le cas en mode "sortie", on utilise la GPIO en mode "entrée".
Pour cela, on doit détecter le bouton poussoir, connecté sur le GPIO18, dans une boucle infinie.

A toutes les 20 ms, on détecte le signal émis par le bouton poussoir.
Si un appuie a été effectué à ce moment là, on a un clignotement de la LED0, sinon ce sera la LED1 qui clignoterait.
	
	/*
	 * Headers that are required for printf and mmap.
	 */
	
	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
	
	#include <fcntl.h>
	#include <sys/mman.h>
	#include <unistd.h>
	
	/*
	 * Base of peripherals and base of GPIO controller.
	 */
	
	#define BCM2835_PERIPH_BASE     0x20000000
	#define BCM2835_GPIO_BASE       ( BCM2835_PERIPH_BASE + 0x200000 )
	
	/*
	 * Paging definitions.
	 */
	
	#define RPI_PAGE_SIZE           4096
	#define RPI_BLOCK_SIZE          4096
	
	/*
	 * Helper macros for accessing GPIO registers.
	 */
	
	#define GPIO_CONF_FUNC_REG( ptr, gpio ) \
	    *( (ptr) + ( (gpio) / 10 ) )
	
	#define GPIO_CONF_FUNC_INPUT_MASK( gpio ) \
	    ( ~( 0x7 << ( ( (gpio) % 10 ) * 3 ) ) )
	
	#define GPIO_CONF_FUNC_OUTPUT_MASK( gpio ) \
	    ( 0x1 << ( ( (gpio) % 10 ) * 3 ) )
	
	#define GPIO_CONF_AS_INPUT( ptr, gpio ) \
	    GPIO_CONF_FUNC_REG( ptr, gpio ) &= GPIO_CONF_FUNC_INPUT_MASK( gpio )
	
	#define GPIO_CONF_AS_OUTPUT( ptr, gpio ) \
	    do { \
	        GPIO_CONF_AS_INPUT( ptr, gpio ); \
	        GPIO_CONF_FUNC_REG( ptr, gpio ) |= \
	            GPIO_CONF_FUNC_OUTPUT_MASK( gpio ); \
	    } while ( 0 )
	
	#define GPIO_CONF_REG( ptr, addr, gpio ) \
	    *( (ptr) + ( ( (addr) / sizeof( uint32_t ) ) + ( (gpio) / 32 ) ) )
	
	#define GPIO_SET_REG( ptr, gpio ) \
	    GPIO_CONF_REG( ptr, 0x1c, gpio )
	
	#define GPIO_CLR_REG( ptr, gpio ) \
	    GPIO_CONF_REG( ptr, 0x28, gpio )
	
	#define GPIO_SET( ptr, gpio ) \
	    GPIO_SET_REG( ptr, gpio ) = 1 << ( (gpio) % 32 )
	
	#define GPIO_CLR( ptr, gpio ) \
	    GPIO_CLR_REG( ptr, gpio ) = 1 << ( (gpio) % 32 )
	
	#define GPIO_VALUE( ptr, gpio ) \
	    ( ( GPIO_CONF_REG( ptr, 0x34, gpio ) >> ( (gpio) % 32 ) ) & 0x1 )
	
	/*
	 * Setup the access to memory-mapped I/O.
	 */
	
	static int mmap_fd;
	
	int setup_gpio_mmap ( uint32_t volatile ** ptr ){
	    void * mmap_result;
	
	    mmap_fd = open ( "/dev/mem", O_RDWR | O_SYNC );
	
	    if ( mmap_fd < 0 ) {
	        return -1;
	    }
	
	    mmap_result = mmap (
	        NULL
	      , RPI_BLOCK_SIZE
	      , PROT_READ | PROT_WRITE
	      , MAP_SHARED
	      , mmap_fd
	      , BCM2835_GPIO_BASE );
	
	    if ( mmap_result == MAP_FAILED ) {
	        close ( mmap_fd );
	        return -1;
	    }
	
	    *ptr = ( uint32_t volatile * ) mmap_result;
	
	    return 0;
	}
	
	void teardown_gpio_mmap ( void * ptr ){
	    munmap ( ptr, RPI_BLOCK_SIZE );
	}
	
	void delay ( unsigned int milisec ){
	    struct timespec ts, dummy;
	    ts.tv_sec  = ( time_t ) milisec / 1000;
	    ts.tv_nsec = ( long ) ( milisec % 1000 ) * 1000000;
	    nanosleep ( &ts, &dummy );
	}
	
	#define min( a, b ) ( a < b ? a : b )
	#define max( a, b ) ( a < b ? b : a )
	
	/*
	 * Main program.
	 */
	
	#define GPIO_LED0   4
	#define GPIO_LED1   17
	#define GPIO_BTN0   18
	
	int main ( int argc, char **argv ){
	
		int period = 1000; /* default = 1Hz */
	    if ( argc > 1 ) {
	        period = atoi ( argv[1] );
	    }
	
	    inputButtonPress(period);
	}
	
	void OutputLed(int period){
		int                 result_4;
	    int                 result_17;
	    int                 half_period;
	    uint32_t volatile * gpio_4 = 4;
	    uint32_t volatile * gpio_17 = 17;
	
	    /* Retreive the mapped GPIO memory. */
	    result_4 = setup_gpio_mmap ( &gpio_4 );
	    result_17 = setup_gpio_mmap ( &gpio_17 );
	
	    if ( result_4 < 0 || result_17 < 0) {
	        printf ( "-- error: cannot setup mapped GPIO.\n" );
	        exit ( 1 );
	    }
	
	
	    half_period = period / 2;
	
	    /* Setup GPIO of LED0 to output. */
	    GPIO_CONF_AS_OUTPUT ( gpio_4, GPIO_LED0 );
	    GPIO_CONF_AS_OUTPUT ( gpio_17, GPIO_LED1 );
	
	    printf ( "-- info: start blinking.\n" );
	
	    /* Blink led at frequency of 1Hz. */
	
	    while (1) {
	        GPIO_SET ( gpio_4, GPIO_LED0 );
	        delay ( half_period );
	        GPIO_CLR ( gpio_4, GPIO_LED0 );
	        delay ( half_period );
	        GPIO_SET ( gpio_17, GPIO_LED1 );
	        delay ( half_period );
	        GPIO_CLR ( gpio_17, GPIO_LED1 );
	        delay ( half_period );
	    }
	    return;
	}
	
	void inputButtonPress(int period) {
	    int                 result_4;
	    int                 result_17;
	    int                 result_18;
	    int                 half_period;
	    uint32_t volatile * gpio_4 = 4;
	    uint32_t volatile * gpio_17 = 17;
	    uint32_t volatile * gpio_18 = 18;
	
	    /* Retreive the mapped GPIO memory. */
	    result_4 = setup_gpio_mmap ( &gpio_4 );
	    result_17 = setup_gpio_mmap ( &gpio_17 );
	    result_17 = setup_gpio_mmap ( &gpio_18 );
	
	    if ( result_18 < 0 ||  result_4 < 0 || result_17 < 0) {
	        printf ( "-- error: cannot setup mapped GPIO.\n" );
	        exit ( 1 );
	    }
	
	    half_period = period / 2;
	
	    /* Setup GPIO of LED0 to input. */
	    GPIO_CONF_AS_INPUT ( gpio_18, GPIO_BTN0 );

	    /* Setup GPIO of LED0 to output. */
	    GPIO_CONF_AS_OUTPUT ( gpio_4, GPIO_LED0 );
	    GPIO_CONF_AS_OUTPUT ( gpio_17, GPIO_LED1 );
	
	    printf ( "-- info: start listening.\n" );
	
	    int val_prec = 1;
			int val_nouv = 1;
			int BP_ON = 0;
			int BP_OFF = 0;
	
			while(1) {
				 delay(20);
				 val_nouv = GPIO_VALUE( gpio_18, GPIO_BTN0 );
	
				 if ((val_prec != val_nouv) && (val_nouv == 0)) {
				 	printf("appui est detecte\n");
				 	BP_ON = 1;
				 }
	
				 if (GPIO_VALUE( gpio_18, GPIO_BTN0 ) == 1) {
				  	BP_OFF = 1;
				 }
	
				 if (BP_ON == 1) {
					BP_ON = 0;                  // l'appui est un evenement ponctuel
					GPIO_SET ( gpio_4, GPIO_LED0 );
		        	delay ( half_period );
		        	GPIO_CLR ( gpio_4, GPIO_LED0 );
		        	delay ( half_period );
				 }
	
				 if (BP_OFF == 1) {
				  	BP_OFF = 0;                // Le relachemet est un evenement ponctuel
				  	GPIO_SET ( gpio_17, GPIO_LED1 );
	        		delay ( half_period );
	        		GPIO_CLR ( gpio_17, GPIO_LED1 );
	        		delay ( half_period );
				 }
			}
	   teardown_gpio_mmap ( &gpio_18 );
	   teardown_gpio_mmap ( &gpio_4 );
	   teardown_gpio_mmap ( &gpio_17 );
	
	   return;
	}

&nbsp;

## 5. Réalisation d'une petite librairie

&nbsp;

Pour alléger les fonctions qu'on écrit et qu'on écrira dans les autres TPs, on crée une librairie de fonction à partir des fonctions et des macros définies au début du TP.

Pour cela, on divisé cette librairie en plusieurs fichiers :
	
**gpio_macros.h :**

	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
	#include <fcntl.h>
	#include <sys/mman.h>
	#include <unistd.h>
	
	/*
	 * Base of peripherals and base of GPIO controller.
	 */
	#define BCM2835_PERIPH_BASE     0x20000000
	#define BCM2835_GPIO_BASE       ( BCM2835_PERIPH_BASE + 0x200000 )
	
	/*
	 * Paging definitions.
	 */
	#define RPI_PAGE_SIZE           4096
	#define RPI_BLOCK_SIZE          4096
	
	/*
	 * Helper macros for accessing GPIO registers.
	 */
	#define GPIO_CONF_FUNC_REG( ptr, gpio ) \
	    *( (ptr) + ( (gpio) / 10 ) )
	
	#define GPIO_CONF_FUNC_INPUT_MASK( gpio ) \
	    ( ~( 0x7 << ( ( (gpio) % 10 ) * 3 ) ) )
	
	#define GPIO_CONF_FUNC_OUTPUT_MASK( gpio ) \
	    ( 0x1 << ( ( (gpio) % 10 ) * 3 ) )
	
	#define GPIO_CONF_AS_INPUT( ptr, gpio ) \
	    GPIO_CONF_FUNC_REG( ptr, gpio ) &= GPIO_CONF_FUNC_INPUT_MASK( gpio )
	
	#define GPIO_CONF_AS_OUTPUT( ptr, gpio ) \
	    do { \
	        GPIO_CONF_AS_INPUT( ptr, gpio ); \
	        GPIO_CONF_FUNC_REG( ptr, gpio ) |= \
	        GPIO_CONF_FUNC_OUTPUT_MASK( gpio ); \
	    } while ( 0 )
	
	#define GPIO_CONF_REG( ptr, addr, gpio ) \
	    *( (ptr) + ( ( (addr) / sizeof( uint32_t ) ) + ( (gpio) / 32 ) ) )
	
	#define GPIO_SET_REG( ptr, gpio ) \
	    GPIO_CONF_REG( ptr, 0x1c, gpio )
	
	#define GPIO_CLR_REG( ptr, gpio ) \
	    GPIO_CONF_REG( ptr, 0x28, gpio )
	
	#define GPIO_SET( ptr, gpio ) \
	    GPIO_SET_REG( ptr, gpio ) = 1 << ( (gpio) % 32 )
	
	#define GPIO_CLR( ptr, gpio ) \
	    GPIO_CLR_REG( ptr, gpio ) = 1 << ( (gpio) % 32 )
	
	#define GPIO_VALUE( ptr, gpio ) \
	    ( ( GPIO_CONF_REG( ptr, 0x34, gpio ) >> ( (gpio) % 32 ) ) & 0x1 )

**gpio_value.c :**

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
	      val == 1 ? GPIO_SET (ptr, gpio) :  GPIO_CLR (ptr, gpio);
	      return 0;
	}

**gpio_setup.c :**

	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <fcntl.h>
	#include <sys/mman.h>
	#include <stdint.h>
	#include <gpio_macros.h>
	
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
	    direction ? GPIO_CONF_AS_INPUT (ptr, gpio) : GPIO_CONF_AS_OUTPUT (ptr, gpio);
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