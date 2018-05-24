# TP7 : Projet - Réseau de capteurs minimaliste

&nbsp;

## Objectifs

- Ecrire le code Arduino pour envoyer les valeurs de capteur de lumière via le NRF24L01.
- Lire les valeurs envoyées depuis le NRF24L01 de l'Arduino et l'envoyer dans la base de donnée SQlite3.
- Récupérer les données de la base de donnée et l'afficher dans un graphe sur le web.

&nbsp;

## Code projet_arduino.ino

Nous avons globalement repris les codes des différents exercices que nous avons fait lors du TP6.

Nous avons repris le concept de la boite aux lettres `mailbox`. Cette boite aux lettres va permettre dans le cas de ce projet de lire la pin 15 du capteur de lumière et de l'écrire dans une structure `Mess_st`. Cette structure `Mess_st` va alors écrire chaque valeur dans l'afficheur OLED pour que nous puissons vérifier le comportement du capteur.

Cette valeur va plus tard être écrite dans le NRF24L01 de l'Arduino pour que le NRF24L01 de la Raspberry puisse le recevoir.

Pour pouvoir écrire dans le NRF24L01, il a fallu faire :

Dans le setup() : 
>	radio.openWritingPipe(addresses[0]);
>	qui permet d'ouvrir un pipe en écriture
>	
>	radio.stopListening();
>	qui permet de stopper l'écoute du NRF24L01 pour permettre ensuite l'ecriture

Dans la loop() : 
>	radio.write( ctx->mess, sizeof(ctx->mess) );
>	qui permet d'écrire dans le NRF le valeur du capteur de lumière précédemment convertit en String 

&nbsp;

## Code NRF24L01_base.cpp