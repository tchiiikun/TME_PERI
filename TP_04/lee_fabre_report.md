# TP4 : Serveur WEB minimaliste pour la commande des LEDs et la lecture du bouton poussoir.

 * browser web :
 * serveur web :
 * requete client :
 * CGI (Common Gateway Interface) :
 * HTML :
 * 

## 1. Communication par FIFO
### writer.py
#### Dans quel répertoire est créée la fifo ?
Dans /tmp/

#### Quelle différence mkfifo et open ?
Creation d'une fifo dans un fichier a partir d'un nom et ouverture d'un fichier
dans le system de fichier.

#### Pourquoi tester que la fifo existe ?
La creation a pu rater.

#### À quoi sert flush ?
Forcer l'ecriture du buffer car print d'ecrit pas vraiment et va d'abord
stocker dans un buffer si on a pas de \n a la fin.

#### Pourquoi ne ferme-t-on pas la fifo ?
Si on ferme la FIFO alors le programme C ne pourra ecrire dedans.

### reader.py

#### Que fait readline ?
Lis une ligne jusqu'a la fin de l'argument size ou jusqu'a un \n

###  Vous allez remarquer que lorsque le vous lancer un écrivain (en C ou en Pyhton) rien ne se passe tant que vous n'avez pas lancé un lecteur. Expliquez le phénomène.
Si on ecrit sans qu'il n'y ait de lecteur alors on tombe en attente.

## 2. Création d'un serveur fake
## 3. Création d'un serveur web
