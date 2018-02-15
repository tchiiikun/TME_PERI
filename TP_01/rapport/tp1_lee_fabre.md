# <center> TP1 : Outils de développement et GPIO </center>

## 0. Plateforme de développement

La Raspberry qui nous a été attribué est la Raspberry 22.

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

Pour pouvoir utiliser le compilateur, on doit le configurer dans le terminal et le vérifier :

	$ source /users/enseig/franck/peri/export_rpi_toolchain.sh
	$ which bcm2708hardfp-gcc

Avant toute chose, il faut écrire un programme C pour afficher **"Hello World !"** :
	
	int main(void) 
	{
		printf("Hello World !\n");
		return 0;
	}

et écrire un **Makefile** pour le compiler et l'envoyer sur la Raspberry.

## 2. Contrôle de GPIO en sortie

Pour pouvoir manipuler une pin du GPIO en mode "sortie" pour contrôler le clignotement d'une LED à une fréquence donnée. 

Pour réaliser ceci, on doit :

* allouer en mémoire à travers **mmap** pour obtenir le pointeur sur la zone mémoire attribué au GPIO 
* configurer la sortie pour connecter la LED à la zone mémoire du GPIO
* allumer la LED en alternant '1' et '0' d'une demi période
* désallouer la zone mémoire

On récupère le dossier qui contient les fichiers nécessaires : 

	cp -rp /users/enseig/franck/peri/lab1

On observe que la LED s'allume et s'éteigne de manière alternée.
	
Pour compléter le code qui nous avez été donné, on a ajouté :

	GPIO_CLR ( gpio_base, GPIO_LED0 ); 
	delay ( half_period );

qui permet de mettre la LED0 à 0 pendant un laps de temps, dans notre cas, ce sera une `half_period`.

## 3. Contrôle de plusieurs GPIO en mode "sortie"

Dans cette partie, il va falloir faire clignoté les deux LEDS : 

* LED0 connecté sur le GPIO4
* LED1 connecté sur le GPIO17
	
Pour cela, on a ajouté :

* une nouvelle LED : `GPIO_LED1` qui aura comme valeur 17 et qui va être connecté à un pointeur `*gpio_base_17`,
* une nouvelle projection en mémoire : `setup_gpio_mmap ( &gpio_base_17 );`,
* la configuration pour connecter le GPIO à la LED1	en sortie : `GPIO_CONF_AS_OUTPUT ( gpio_base_17, GPIO_LED1 );`
* les commandes pour clignoter la LED1 avec `GPIO_SET` et `GPIO_CLR` 

On a choisi d'alterner les deux LEDS : pendant une période, la première LED clignote, puis à la prochaine période, la deuxième LED clignote.

## 4. Lecture de la valeur d'une entrée GPIO

Après avoir traiter le cas en mode "sortie", on utilise la GPIO en mode "entrée".

Pour cela, on doit détecter le bouton poussoir, connecté sur le GPIO18, dans une boucle infinie.

A toutes les 20 ms, on détecte le signal émis par le bouton poussoir.
Si un appuie a été effectué à ce moment là, la LED0 clignore, sinon ce sera la LED1 qui clignotera.
	
Pour cela, on a ajouté :

* une nouvelle BTN : `GPIO_BTN0` qui aura comme valeur 18 et qui va être connecté à un pointeur `*gpio_base_18`,
* une nouvelle projection en mémoire : `setup_gpio_mmap ( &gpio_base_18 );`,
* la configuration pour connecter le GPIO à la LED1	en entrée : `GPIO_CONF_AS_INPUT ( gpio_base_18, GPIO_BTN0 );`,
* les commandes pour clignoter la LED1 avec `GPIO_SET` et `GPIO_CLR` selon l'appui sur le bouton poussoir.

## 5. Réalisation d'une petite librairie

Pour alléger les fonctions qu'on écrit et qu'on écrira dans les autres TPs, on crée une librairie de fonction à partir des fonctions et des macros définies au début du TP.

Pour cela, on a divisé cette librairie en plusieurs fichiers :
	
* **gpio_macros.h** contenant tous les macros
* **gpio_value.h / gpio_value.c** contenant les fonctions :
	* `int gpio_read(int gpio, int * val, uint32_t volatile*  ptr);` qui permet de récupérer `GPIO_VALUE`,
	* `int gpio_update(int gpio, int val, uint32_t volatile*  ptr);` qui permet de mettre à jour l'état de la broche en choisissant entre `GPIO_SET` et `GPIO_CLR`.
* **gpio_setup.c** contenant les fonctions : 
	* `int setup_gpio_mmap(uint32_t volatile ** ptr);` qui permet d'établire une projection en mémoire de la broche.
	* `int gpio_setup_io(int gpio, int direction, uint32_t volatile*  ptr);` qui permet la configuration des broches en INPUT ou OUTPUT,
	* `void teardown_gpio_mmap(void * ptr);` qui permet de supprimer la projection,
	* `void delay(unsigned int milisec);` qui permet de faire un sleep.

Pour vérifier que la librairie, qu'on a crée, fonctionne, on a récupéré le code de la seconde partie et on a ajouté les différentes parties de la librairie.

