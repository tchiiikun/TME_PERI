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

## 2 et 3. Création d'un serveur web et gestion du serveur fake

Nous avons refait toute la gestion du serveur fake et modifie le script python
du server.py. Le serveur fake va dans un premier temps ouvrir les descripteurs
de fichiers dont il a besoin pour lire et pour ecrire. Il lit sur le bouton
poussoir ou sur stdin par exemle et ecrit sur les led ou sur l'ecran ou sur
stdout. Nous avons fait une sous fonction qui gere le serveur fake. Ceci permet
une abstraction et une bien meilleur reutilisation du code.

Dans la foncton server :
- fd_in : l'entree sur laquelle on lit.
- fd_out : la sortie sur laquelle o ecrit 
- bp2f2s : buffer qui transfert depuis le bouton poussoir vers fake vers server.
pour lui renvoyer le resultat (utile pour le projet).
- s2f2led : buffer qui transfert depuis le server.py vers fake qui l'erira sur la
LED.
int server(int fd_in, int fd_out, char *bp2f2s, char *s2f2led);

La fonction fait :
declaration de toutes les variables, des fifos pour comminiquer entre server et
fakem open des fifo. Une boucle while pour verifier a coup de select si quelque
chose se pase sur le buffer fd_in et sur s2f (server to fake), on verifie en
fait si quelqu'un appuie sur le bouton et quelqu'un ecrit une valeur depuis le
site web.

On va utiliser une methode a deux booleens pour verifier si quelqu'un a ecrit
dans stdin. Pour ne pas bloquer le programme on verifie avec un premier
booleen si a ce tour de boucle quelqu'un a ecrit (on peut le savoir grace au
select) et on met le booleen bool_fd_in a true. on va donc lire et preparer le
tour suivant en mettant un second booleen a true qui previent qu'au tour actuel
(le futur tour precedent) quelque chose a ete ecrit et on remet bool_fd_in a 0
pour qu'il attende la prochaine ecriture.
Pour ecrire sur la LED il faut remplir deux conditions, que quelque chose soit
ecrit sur stdin (le bouton poussoir) ET sur le site web (serve.py).
Il faudra quand meme faire attention, lorsqu'on utilise le bouton poussoir on
nous envoie en rafale des informations et le code est a changer car on doit
verifier non pas si quelque chose a ete ecrit mais si il y a eu un changement
entre le buffer a ce tour ci et le buffer precedent.
Le bouton etant casse nous ne pouvons pas faire ce teste mais une
implementation est en commentaire dans le code.


