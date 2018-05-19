#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NBLED 2
#define NBBP 1
char led[NBLED];
char bp[NBBP];
 
int main()
{
   int i;
   int fd = open("/dev/ledbpLF", O_RDWR);
   if (fd < 0) {
      fprintf(stderr, "Erreur d'ouverture du pilote LED et Boutons\n");
      exit(1);
   }
   for( i = 0; i < NBLED; i ++) {
      led[i] = '0';
   }
   while(1) {
      read( fd, bp, 1);
      
      if(bp[0] == '1') {
      	led[1] = '0';
      	led[0] = (led[0] == '0') ? '1' : '0';
		    write( fd, led, 2);
		    sleep(1);
		  } else {
		  	led[0] = '0';
		  	led[1] = (led[1] == '0') ? '1' : '0';
		    write( fd, led, 2);
		    sleep(1);
		  }
   } 
   return 0;
}
