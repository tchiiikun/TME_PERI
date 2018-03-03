# TP$ : Serveur WEB minimaliste pour la commande des LEDs et la lecture du bouton poussoir.


## Communication par FIFO

### Dans quel répertoire est créée la fifo ?
Dans le repertoire /tmp pour des dossiers temporaires.

### Quelle difference mkfifo et open?
mkfifo cree un fichier et le defini comme une fifo et open cree une ouverture de fichier dans le systeme file system.

### Pourquoi tester que la FIFO existe?
Car il peut y avoir une erreur a la creation si on a deja cree une fifo ou un
fichier aillant le meme nom.

### A quoi sert flush?
A forcer le programme a ecrire car la fonction write fait une ecriture qui est
buffurise par le systeme.

### Pourquoi ne ferme-t-on pas la FIFO?
Car on va l'utiliser avec un systeme client-serveur

### Que fait readline?
La fonction readline() va lire une ligne jusqu'a \n dans le fifo.

### Expliquez le phenomene?
Dans un producteur consommateur si tout le monde produit mais personne ne
consomme, il ne se passe rien.

## Creation d'un fake serveur
// TODO recopier le code et expliquer pourquoi il faut utiliser readline avec
// \n et pourquoi il faut utiliser read(nb char)

## Creation d'un serveur web 

###





