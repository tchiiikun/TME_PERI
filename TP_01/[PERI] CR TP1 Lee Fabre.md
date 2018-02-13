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

	CC=bcm2708hardfp-gcc
	CFLAGS=-O2 -static
	CARD=22
	NAME=lee-fabre
	APP=helloworld
	
	all: $(APP)
		scp -P 50$(CARD) $^ pi@132.227.102.36:lee-fabre
	
	clean:
		rm $(APP)

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

Cf voir : *lab1_ex1.c*

On observe que la LED s'allume et d'éteigne de manière alterner.

## 3. Contrôle de plusieurs GPIO en mode "sortie"

&nbsp;

Dans cette partie, on doit faire clignoté les deux LEDS : 

* LED0 connecté sur le GPIO4
* LED1 connecté sur le GPIO17

On a choisi d'alterner les deux LEDS, lorsqu'une des LEDS s'allume, l'autre LED s'éteigne.

## 4. Lecture de la valeur d'une entrée GPIO

&nbsp;

Dans cette partie, on doit utiliser la GPIO en mode entrée, détecter le bouton poussoir dans une boucle infinie.


## 5. Réalisation d'une petite librairie


&nbsp;

Dans cette partie, créer une librairie à partir des fonctions et des macros définies au début du tp.





