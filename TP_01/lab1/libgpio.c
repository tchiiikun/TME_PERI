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

int setup_gpio_mmap ( uint32_t volatile ** ptr )
{
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

void teardown_gpio_mmap ( void * ptr )
{
    munmap ( ptr, RPI_BLOCK_SIZE );
}

void delay ( unsigned int milisec )
{
    struct timespec ts, dummy;
    ts.tv_sec  = ( time_t ) milisec / 1000;
    ts.tv_nsec = ( long ) ( milisec % 1000 ) * 1000000;
    nanosleep ( &ts, &dummy );
}

int gpio_setup (int gpio, int direction, uint32_t volatile*  ptr) {

		if (direction == 0 && (gpio >= 3 && gpio <= 26 && gpio != 4 && gpio != 6 && gpio != 9 && gpio != 14 && gpio != 17 && gpio != 20 && gpio != 25)) {
			GPIO_CONF_AS_INPUT ( ptr, gpio );
			return 0;
		} else {
			if (direction == 1 && (gpio >= 3 && gpio <= 26 && gpio != 4 && gpio != 6 && gpio != 9 && gpio != 14 && gpio != 17 && gpio != 20 && gpio != 25)) {
				GPIO_CONF_AS_OUTPUT ( ptr, gpio );
				return 0;
			} else {
				return -1;
			}
		}
}


int gpio_read (int gpio, int * val, uint32_t volatile*  ptr) {

		if (gpio >= 3 && (gpio >= 3 && gpio <= 26 && gpio != 4 && gpio != 6 && gpio != 9 && gpio != 14 && gpio != 17 && gpio != 20 && gpio != 25)) {
			*val = GPIO_VALUE(ptr, gpio);
			return 0;
		} else {
			return -1;
		}
}

int gpio_update (int gpio, int val, uint32_t volatile*  ptr) {

		if (val == 1 && (gpio >= 3 && gpio <= 26 && gpio != 4 && gpio != 6 && gpio != 9 && gpio != 14 && gpio != 17 && gpio != 20 && gpio != 25)) {
			GPIO_SET ( ptr, gpio );
			return 0;
		} else {
			if (val == 0 && (gpio >= 3 && gpio <= 26 && gpio != 4 && gpio != 6 && gpio != 9 && gpio != 14 && gpio != 17 && gpio != 20 && gpio != 25)) {
				GPIO_CLR ( ptr, gpio );
				return 0;
			} else {
				return -1;
			}
		}
}
