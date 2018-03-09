#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NBLED 2
#define NBBP 1
char led[NBLED];
char bp[NBBP];
 
int main()
{
   int i = 0;
   int fd = open("/dev/ledbpLF", O_RDWR);
   if (fd < 0) {
      fprintf(stderr, "Erreur d'ouverture du pilote LED et Boutons\n");
      exit(1);
   }
   do { 
	
	read(fd, bp, 1);

	if ( bp[0] == '1') {	
		/* Clignotement si appui */
		led[0] = '0';
		write(fd, led, 1);
		sleep(1);
		led[0] = '1';
		write(fd, led, 1);
	} else {
		/* Clignotement si non appui */
		led[1] = '0';
		write(fd, led, 1);
		sleep(1);
		led[1] = '1';
		write(fd, led, 1);
	}

	sleep(1);
   } while (1);
   return 0;
}
