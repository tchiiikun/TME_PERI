#include <stdio.h>

#define NBLED 2
#define NBBP 1
char led[NBLED];
char bp[NBBP];
 
int main()
{
   int i;
   int fd = open("/dev/ledbpXY", O_RDWR);
   if (fd < 0) {
      fprintf(stderr, "Erreur d'ouverture du pilote LED et Boutons\n");
      exit(1);
   }
   for( i = 0; i < NBLED; i ++) {
      led[i] = '0';
   }
   do { 
      led[O] = (led[O] == '0') ? '1' : '0';
      write( fd, led, 1);
      sleep( 1);
      read( fd, bp, 1);
   } while (bp[0] == '1');
   return 0;
}
