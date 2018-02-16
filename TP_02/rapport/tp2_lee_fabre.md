# <center> TP2 : Premier pilote  </center> 

&nbsp;

## Objectif

L'objectif de la séance est de commander les LEDS et le bouton poussoir (BP) en passant par un pilote installé dans le noyau. 

## Étape 1 : crétation et test d'un noyau

* La fonction "static int __init mon_module_init(void)" est exécutée lorsqu'on insère le module du noyau.

* La fonction "static void __exit mon_module_cleanup(void)" est exécutée lorsqu'on enlève le module du noyau.

	sudo insmod.module.ko
	lsmod
	dmesg 
	> Hello World
	sudo rmmod module
	lsmod
	dmesg 
	> Goodbye World

## Étape 2 : ajout des paramètres au module

	sudo insmod ./module.ko LED=2 BUT=1
	> Hello World
	> LED=2
	> BUT=1

## Étape 3 : création d'un driver qui ne fait rien mais qui le fait dans le noyau



## Étape 4 : accès aux GPIO depuis les fonctions du pilote



## Étape 5 (optionnel) : Usage d'un timer dans le noyau pour faire clignoter la led


