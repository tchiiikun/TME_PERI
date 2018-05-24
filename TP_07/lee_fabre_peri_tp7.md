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

	radio.openWritingPipe(addresses[0]);
	permet d'ouvrir un pipe en écriture
	
	radio.stopListening();
	permet de stopper l'écoute du NRF24L01 pour permettre ensuite l'ecriture

Dans la loop() : 

	radio.write( ctx->mess, sizeof(ctx->mess) );
	permet d'écrire dans le NRF24L01 le valeur du capteur de lumière précédemment convertit en String 

&nbsp;

## Code NRF24L01_base.cpp

Dans le fichier NRF24L01_base.cpp, nous lisons les données envoyées par le NRF24L01 de l'Arduino avec les commandes qui ne lisent seulement si une donnée est valable :

	if( radio.available()){
		radio.read( buffer, sizeof(buffer) );	
	}

Nous écrivons par la suite dans une base de données les valeurs du capteur de lumière et le temps actuel au format Heure:Minutes:Secondes, qui va nous être utile pour dessiner le graphe en fonction du temps.

Nous avons utilisé différentes fonctions de SQlite3 pour créer notre base de données :

Dans un premier temps, nous avons crée une table de la base de donnée avec :

	sqlite3_exec(db, "CREATE TABLE Ard(time INT, lumiere INT);", 0, 0, &errmsg);

qui contient deux colonnes, une pour le temps actuel et l'autre pour la donnée du capteur de lumière.

Ensuite dans une boucle infinie, chaque valeur va être insérée dans la base de donnée dans la colonne dédiée: 

	sprintf(query, "INSERT INTO Ard VALUES(strftime('%s', 'now'), %d);","%s", verification);
	sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	sqlite3_step(stmt);
	
Dans cette première ligne on peut voir qu'il nous faut utiliser un `sprintf` pour écrire la query dans une string qui elle sera pasée à une fonction C qui va l'envoyer à la database. Le première paramètre de sprintf est "%s" car sprintf essayait d'écrire le second paramètre dans le premier '%s' de la commande sql `strftime` qui donne le temps depuis `EPOCH`.
`INSERT` va insérer dans la base de donnée nommée Ard les valeurs en paramètre.
Nous avons fait ce programme en C car nous lisions déjà en C les informations envoyées par l'Arduino et il était assez pratique de les écrire directement dans une databse avec `SQLITE3, l'interface est assez claire et nos opérations sont très simples.
On Ouvre la DB, on remplie les champs pour le temps et la valeur du capteurs pour avoir une `timestamp` par résultat. Puis on appelle loop qui va remplir la DB en `temps réel`.
Petit soucis l'arduino renvoie parfois des valeurs mauvaises à 0 (impossible) ou plusieurs milliards (impossible) alors on sauvegarde la valeur précédente lu par la raspberry et si la valeur actuelle est trop extrême alors on la remplace par la valeur précédente.


&nbsp;

## Code de la partie Web

### Fichier web.py :

Ce fichier utilise `Flask` qui est un framework de développement web en Python permettant d'utiliser des templates et `JSON` qui est un format de données textuelles dérivé de la notation des objets du langage JavaScript. l'interface proposée par Flask est simple : un dossier templates, un dossier static et un fichier qui sert a serveur web en python et qui appellera dans templates la page à générer et déclarera une fonnction et un point d'accès vers cette page/fonction dans l'URL. Elle va aussi déclarer une fonnction pour appeler les fichiers json qui eux seront utilisé dans une div du template en HTML pour afficher par exemple ici un graph.

Nous avons défini deux fonctions dans le fichier python:

- data() permet de récupérer les données de la base de données 

		cursor.execute("SELECT 1000*time, lumiere from Ard") 	// Va récupérer la donnnée dans la base de donnée et
									// multiplie par 1000 pour tenter de corriger l'heure
									// mais en vain.

- graph() permet de faire faire appel au fichier HTML : graph.html

	 	return render_template('graph.html') // Va renvoyer vers la page graph.html si l'URL est XXXXXX/graph

### Fichier graph.html : 

Ce fichier permet de faire afficher sous la forme d'un graphe les données récupérées.
C'est une fichier HTML simple avec du json qui sert a mettre le graphique dans le body. Nous avons récupérer ce fichier avec un template préfait pour flask ce qui fut extrêmement pratique pour implémenter notre solution.
