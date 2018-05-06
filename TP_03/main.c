#include <stdio.h>
#include <stdlib.h>

int main() {

	char* tab = "Hello World !";

	// ouverture du fichier
	int fd = open("/dev/ledbpLF", O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "Erreur d'ouverture du pilote LED et Boutons\n");
		exit(1);
	}


	// affichage du Hello World
	lcl_init(); // initialisation du LCD

	lcd_message(tab); // affichage

	lcl_clear(); // fermeture
}

