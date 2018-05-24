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
>	permet d'ouvrir un pipe en écriture
>	
>	radio.stopListening();
>	permet de stopper l'écoute du NRF24L01 pour permettre ensuite l'ecriture

Dans la loop() : 
>	radio.write( ctx->mess, sizeof(ctx->mess) );
>	permet d'écrire dans le NRF24L01 le valeur du capteur de lumière précédemment convertit en String 

&nbsp;

## Code NRF24L01_base.cpp

Dans le fichier NRF24L01_base.cpp, nous écrivons dans une base de données les valeurs du capteur de lumière et le temps actuel au format Heure:Minutes:Secondes, qui va nous être utile pour dessiner le graphe en fonction du temps.

Dans un premier temps, nous avons crée une table de la base de donnée avec :
`sqlite3_exec(db, "CREATE TABLE Ard(time INT, lumiere INT);", 0, 0, &errmsg);` 
qui contient deux colonnes, une pour le temps actuel et l'autre pour la donnée du capteur de lumière.

Ensuite dans une boucle infinie, chaque valeur va être insérée dans la base de donnée dans la colonne dédiée: 
`sprintf(query, "INSERT INTO Ard VALUES(strftime('%s', 'now'), %d);","%s", verification);`

Nous avons utilisé différentes fonctions de SQlite3 pour créer notre base de données :

&nbsp;

## Code de la partie Web

Fichier web.py : 

Fichier graph.html : 