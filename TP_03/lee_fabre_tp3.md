# <center> TP3 : Pilotage d'un écran LCD en mode utilisateur et par un driver  </center> 

&nbsp;

## Objectif

 Objectifs:

   * Configuration des GPIOs pour le pilotage d'un périphérique
   * Fonctionnement d'un écran LCD et fonctions de base
   * Affichage du monitoring système
   * Ecriture d'un driver d'affichage 

## 1. Configuration des GPIO pour le pilotage de l'écran

### Question 1: OK
	// met E a un. atttends la propagation et passage a 0
	void lcd_strobe(void)
	{
	  gpio_update(E,1);
	  udelay(1000);
	  gpio_update(E,0);
	  
	}


### Question 2:
	// Chargement des bits de poids fort (macro BIT)
	// lcd_strobe pour envoyer vers le LCD
	// Chargement des bits de poids faible
	// lcd_strobe pour envoyer vers le LCD

	// Macro HEX(h,l):
		// concatenation de chaine de caractere.
### Question 3: 
Void commentaire code.
### Question 4: OK
### Question 5: OK

## 2. Fonctionnement de l'écran et fonctions de base

Utilisation de lcd_message(char* message) pour afficher helloworld.
Lit chaque char de la chaine et les envoie vers lcd_data.
lcd_data met RS a 1 (ecriture) puis appelle lcd_write4bits.

## 3. Affichage du monitoring système
### Question 1:
On open le fd de /proc/loadavg (c'est un fichier du fs /proc)
utilisation cmme si c'etait un simple fichier grace a l'implementation de
procfs.

## 4. Driver basique pour le LCD
Pour creer un driver :
Une fonction init, une fonction exitm register le node dans l'arbre des
devices. implementation d'une struct file_operation. les fops utiliseront le
lcd_message et le lcd_write.
